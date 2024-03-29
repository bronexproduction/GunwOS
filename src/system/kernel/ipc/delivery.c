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

static enum gnwIpcError validateQuery(const struct gnwIpcSenderQuery absQuery) {
    if (!absQuery.pathData.ptr) {
        OOPS("IPC path nullptr", GIPCE_UNKNOWN);
    }
    if ((absQuery.data.ptr != nullptr) != (absQuery.data.bytes != 0)) {
        OOPS("IPC query data inconsistency", GIPCE_UNKNOWN);
    }
    if (!absQuery.pathData.bytes || absQuery.pathData.bytes > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }

    return GIPCE_NONE;
}

static enum gnwIpcError validateNotificationQuery(const struct gnwIpcSenderQuery absQuery) {
    enum gnwIpcError e = validateQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    if (!k_ipc_utl_pathNotificationValidate(absQuery.pathData)) {
        return GIPCE_INVALID_PATH;
    }
    if (absQuery.replyData.ptr || absQuery.replyErrPtr || absQuery.replyData.bytes) {
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
                                                             sizeof(struct gnwIpcEndpointQuery) + queryPtr->data.bytes,
                                                             sizeof(struct gnwIpcEndpointQuery),
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
                            const struct gnwIpcSenderQuery absQuery) {
    
    enum gnwIpcError e = validateQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    enum gnwIpcListenerType type = k_ipc_utl_pathGlobalValidate(absQuery.pathData) ? GILT_GLOBAL : GILT_NONE;
    type |= k_ipc_utl_pathDirectValidate(absQuery.pathData) ? GILT_DIRECT : GILT_NONE;

    if (__builtin_popcount(type) != 1) {
        return GIPCE_INVALID_PATH;
    }

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.pathData, nullptr);
    const struct ipcListener * listenerPtr = nullptr;

    while (listenerIndex < MAX_IPC_LISTENER) {
        listenerPtr = &ipcListenerRegister[listenerIndex];

        if (type == GILT_GLOBAL) {
            break;
        } else if ((type == GILT_DIRECT) && (listenerPtr->procId == absQuery.procId)) {
            break;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.pathData, &listenerIndex);
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
    endpointQuery.data = absQuery.data;
    endpointQuery.replySizeBytes = absQuery.replyData.bytes;
    endpointQuery.bound = listenerPtr->bindingRequired;
    endpointQuery.permissions = permissions;

    if (endpointQuery.replySizeBytes) {
        if (!absQuery.replyData.ptr) {
            OOPS("Unexpected null pointer to IPC reply data", GIPCE_UNKNOWN);
        }
        if (!absQuery.replyErrPtr) {
            OOPS("Unexpected null pointer to IPC reply error", GIPCE_UNKNOWN);
        }

        endpointQuery.token = freeReplyIndex();
        if (endpointQuery.token >= MAX_IPC_TOKEN) {
            OOPS("IPC reply table full", GIPCE_UNKNOWN);
        }

        ipcReplyRegister[endpointQuery.token].senderProcId = procId;
        ipcReplyRegister[endpointQuery.token].handlerProcId = listenerPtr->procId;
        ipcReplyRegister[endpointQuery.token].absReplyErrorPtr = absQuery.replyErrPtr;
        ipcReplyRegister[endpointQuery.token].absReplyBufferData.ptr = absQuery.replyData.ptr;
        ipcReplyRegister[endpointQuery.token].absReplyBufferData.bytes = endpointQuery.replySizeBytes;
        
        k_proc_lock(procId, PLT_IPC);
    } else {
        endpointQuery.token = MAX_IPC_TOKEN;
    }

    e = deliver(listenerPtr, &endpointQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    switch (absQuery.bindData.flag) {
        case GIBF_BIND:
            e = k_ipc_binding_create(listenerPtr->procId, procId, absQuery.bindData.permissions);
            if (e == GIPCE_ALREADY_EXISTS) {
                e = GIPCE_NONE;
            }
            break;
        case GIBF_UPDATE:
            e = k_ipc_binding_update(listenerPtr->procId, procId, absQuery.bindData.permissions);
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

enum gnwIpcError k_ipc_notify(const struct gnwIpcSenderQuery absQuery,
                              const procId_t target) {

    enum gnwIpcError e = validateNotificationQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    LOG2("ipc_notify: path - ", (gnwIpcPath)absQuery.pathData.ptr);

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.pathData, nullptr);
    while (listenerIndex < MAX_IPC_LISTENER) {
        if (ipcListenerRegister[listenerIndex].procId == target) {
            break;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.pathData, &listenerIndex);
    }
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NONE;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = KERNEL_PROC_ID;
    endpointQuery.data = absQuery.data;
    endpointQuery.replySizeBytes = 0;
    endpointQuery.bound = 0;
    endpointQuery.permissions = 0;
    endpointQuery.token = MAX_IPC_TOKEN;

    return deliver(&ipcListenerRegister[listenerIndex], &endpointQuery);
}
