//
//  delivery.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "ipc.h"
#include "data.h"
#include "binding.h"
#include "utility.h"
#include <defs.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <log/log.h>
#include <queue/queue.h>

static size_t freeReplyIndex() {
    for (size_t index = 0; index < MAX_IPC_TOKEN; ++index) {
        if (ipcReplyRegister[index].senderProcId == NONE_PROC_ID) {
            return index;
        }
    }

    return MAX_IPC_TOKEN;
}

static enum gnwIpcError validateQuery(const struct gnwIpcSenderQuery query) {
    if (!query.pathData.ptr) {
        OOPS("IPC path nullptr", GIPCE_UNKNOWN);
    }
    if ((query.data.ptr != nullptr) != (query.data.bytes != 0)) {
        OOPS("IPC query data inconsistency", GIPCE_UNKNOWN);
    }
    if (!query.pathData.bytes || query.pathData.bytes > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }

    return GIPCE_NONE;
}

static enum gnwIpcError validateNotificationQuery(const struct gnwIpcSenderQuery query) {
    enum gnwIpcError e = validateQuery(query);
    if (e != GIPCE_NONE) {
        return e;
    }

    if (!k_ipc_utl_pathNotificationValidate(query.pathData)) {
        return GIPCE_INVALID_PATH;
    }
    if (query.replyData.ptr || query.replyErrPtr || query.replyData.bytes) {
        /*
            Replying to broadcast events not supported
        */
        return GIPCE_INVALID_PARAMETER;
    }

    return GIPCE_NONE;
}

static enum gnwIpcError deliver(const struct ipcListener * const listenerPtr,
                                const struct gnwIpcEndpointQuery * const queryPtr) {

    const enum k_proc_error err = k_proc_callback_invoke_ptr(listenerPtr->procId,
                                                             (gnwEventListener_ptr)listenerPtr->listener,
                                                             (ptr_t)queryPtr,
                                                             sizeof(struct gnwIpcEndpointQuery),
                                                             sizeof(struct gnwIpcEndpointQuery) + queryPtr->data.bytes,
                                                             (gnwRunLoopDataEncodingRoutine)gnwIpcEndpointQuery_encode,
                                                             (gnwRunLoopDataEncodingRoutine)listenerPtr->decoder);
    if (err != PE_NONE) {
        if (queryPtr->replySizeBytes) {
            k_ipc_utl_clearReply(queryPtr->token);
            k_que_dispatch_arch((fPtr_arch)k_ipc_utl_unlockIfAble, queryPtr->sourceProcId);
        }

        if (err == PE_IGNORED) {
            return (listenerPtr->type == GILT_NOTIFICATION) ? GIPCE_UNKNOWN : GIPCE_IGNORED;
        } else if (err == PE_LIMIT_REACHED) {
            return GIPCE_FULL;
        } else {
            return GIPCE_UNKNOWN;
        }
    }

    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_send(const procId_t procId,
                            const struct gnwIpcSenderQuery query) {
    
    enum gnwIpcError e = validateQuery(query);
    if (e != GIPCE_NONE) {
        return e;
    }

    enum gnwIpcListenerType type = k_ipc_utl_pathGlobalValidate(query.pathData) ? GILT_GLOBAL : GILT_NONE;
    type |= k_ipc_utl_pathDirectValidate(query.pathData) ? GILT_DIRECT : GILT_NONE;

    if (__builtin_popcount(type) != 1) {
        return GIPCE_INVALID_PATH;
    }

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(query.pathData, nullptr);
    const struct ipcListener * listenerPtr = nullptr;

    while (listenerIndex < MAX_IPC_LISTENER) {
        listenerPtr = &ipcListenerRegister[listenerIndex];

        if (type == GILT_GLOBAL) {
            break;
        } else if ((type == GILT_DIRECT) && (listenerPtr->procId == query.procId)) {
            break;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(query.pathData, &listenerIndex);
    }
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NONE;
    }

    if (!listenerPtr->bindingRequired && (type == GILT_DIRECT)) {
        return GIPCE_INVALID_PARAMETER;
    }

    size_t permissions = 0;
    if (listenerPtr->bindingRequired) {
        e = k_ipc_binding_getPermissions(procId, listenerPtr->procId, &permissions);
        if (e != GIPCE_NONE) {
            return GIPCE_FORBIDDEN;
        }
        if ((listenerPtr->permissionMask & permissions) != listenerPtr->permissionMask) {
            return GIPCE_FORBIDDEN;
        }
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = procId;
    endpointQuery.data = query.data;
    endpointQuery.replySizeBytes = query.replyData.bytes;
    endpointQuery.bound = listenerPtr->bindingRequired;
    endpointQuery.permissions = permissions;

    if (endpointQuery.replySizeBytes) {
        if (!query.replyData.ptr) {
            OOPS("Unexpected null pointer to IPC reply data", GIPCE_UNKNOWN);
        }
        if (!query.replyErrPtr) {
            OOPS("Unexpected null pointer to IPC reply error", GIPCE_UNKNOWN);
        }

        endpointQuery.token = freeReplyIndex();
        if (endpointQuery.token >= MAX_IPC_TOKEN) {
            OOPS("IPC reply table full", GIPCE_UNKNOWN);
        }

        ipcReplyRegister[endpointQuery.token].senderProcId = procId;
        ipcReplyRegister[endpointQuery.token].handlerProcId = listenerPtr->procId;
        ipcReplyRegister[endpointQuery.token].replyErrorPtr = query.replyErrPtr;
        ipcReplyRegister[endpointQuery.token].replyBufferData.ptr = query.replyData.ptr;
        ipcReplyRegister[endpointQuery.token].replyBufferData.bytes = endpointQuery.replySizeBytes;
        
        k_proc_lock(procId, PLT_SYNC);
    } else {
        endpointQuery.token = MAX_IPC_TOKEN;
    }

    e = deliver(listenerPtr, &endpointQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    switch (query.bindData.flag) {
        case GIBF_BIND:
            e = k_ipc_binding_create(listenerPtr->procId, procId, query.bindData.permissions);
            if (e == GIPCE_ALREADY_EXISTS) {
                e = GIPCE_NONE;
            }
            break;
        case GIBF_UPDATE:
            e = k_ipc_binding_update(listenerPtr->procId, procId, query.bindData.permissions);
            break;
        case GIBF_UNBIND:
            e = k_ipc_binding_destroy(listenerPtr->procId, procId, procId);
            break;
        default:
            break;
    }

    if (e != GIPCE_NONE) {
        return e;
    }
    
    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_notify(const struct gnwIpcSenderQuery query,
                              const procId_t target) {

    enum gnwIpcError e = validateNotificationQuery(query);
    if (e != GIPCE_NONE) {
        return e;
    }

    LOG2("ipc_notify: path - ", (gnwIpcPath)query.pathData.ptr);

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(query.pathData, nullptr);
    while (listenerIndex < MAX_IPC_LISTENER) {
        if (ipcListenerRegister[listenerIndex].procId == target) {
            break;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(query.pathData, &listenerIndex);
    }
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NONE;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = KERNEL_PROC_ID;
    endpointQuery.data = query.data;
    endpointQuery.replySizeBytes = 0;
    endpointQuery.bound = 0;
    endpointQuery.permissions = 0;
    endpointQuery.token = MAX_IPC_TOKEN;

    return deliver(&ipcListenerRegister[listenerIndex], &endpointQuery);
}
