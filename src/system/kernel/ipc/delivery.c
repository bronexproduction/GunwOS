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
    if (!absQuery.path || !absQuery.dataPtr) {
        OOPS("Nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!absQuery.dataSizeBytes) {
        OOPS("No data");
        return GIPCE_UNKNOWN;
    }
    if (!absQuery.pathLen || absQuery.pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }

    return GIPCE_NONE;
}

static enum gnwIpcError validateNotificationQuery(const struct gnwIpcSenderQuery absQuery) {
    enum gnwIpcError e = validateQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    if (!k_ipc_utl_pathIsNotification(absQuery.path, absQuery.pathLen)) {
        return GIPCE_INVALID_PATH;
    }
    if (absQuery.replyPtr || absQuery.replyErrPtr || absQuery.replySizeBytes) {
        /*
            Replying to broadcast events not supported
        */
        return GIPCE_INVALID_PARAMETER;
    }

    return GIPCE_NONE;
}

static enum gnwIpcError deliver(const struct ipcListener * const listenerPtr,
                                const struct gnwIpcEndpointQuery * const queryPtr,
                                const bool isNotification) {

    const enum k_proc_error err = k_proc_callback_invoke_ptr(listenerPtr->procId,
                                                             (gnwEventListener_ptr)listenerPtr->listener,
                                                             (ptr_t)queryPtr,
                                                             sizeof(struct gnwIpcEndpointQuery) + queryPtr->dataSizeBytes,
                                                             sizeof(struct gnwIpcEndpointQuery),
                                                             (gnwRunLoopDataEncodingRoutine)gnwIpcEndpointQuery_encode,
                                                             (gnwRunLoopDataEncodingRoutine)listenerPtr->decoder);
    if (err != PE_NONE) {
        if (queryPtr->replySizeBytes) {
            k_ipc_utl_clearReply(queryPtr->token);
            k_que_dispatch_arch((fPtr_arch)k_ipc_utl_unlockIfAble, queryPtr->sourceProcId);
        }

        if (err == PE_IGNORED) {
            return isNotification ? GIPCE_UNKNOWN : GIPCE_IGNORED;
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

    if (k_ipc_utl_pathIsNotification(absQuery.path, absQuery.pathLen)) {
        return GIPCE_INVALID_PATH;
    }

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.path, absQuery.pathLen, nullptr);
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NOT_FOUND;
    }

    const struct ipcListener * listenerPtr = &ipcListenerRegister[listenerIndex];
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
    endpointQuery.dataPtr = absQuery.dataPtr;
    endpointQuery.dataSizeBytes = absQuery.dataSizeBytes;
    endpointQuery.replySizeBytes = absQuery.replySizeBytes;
    endpointQuery.bound = listenerPtr->bindingRequired;
    endpointQuery.permissions = permissions;

    if (endpointQuery.replySizeBytes) {
        if (!absQuery.replyPtr) {
            OOPS("Unexpected null pointer to IPC reply data");
            return GIPCE_UNKNOWN;
        }
        if (!absQuery.replyErrPtr) {
            OOPS("Unexpected null pointer to IPC reply error");
            return GIPCE_UNKNOWN;
        }

        endpointQuery.token = freeReplyIndex();
        if (endpointQuery.token >= MAX_IPC_TOKEN) {
            OOPS("IPC reply table full");
            return GIPCE_UNKNOWN;
        }

        ipcReplyRegister[endpointQuery.token].senderProcId = procId;
        ipcReplyRegister[endpointQuery.token].handlerProcId = listenerPtr->procId;
        ipcReplyRegister[endpointQuery.token].absReplyErrorPtr = absQuery.replyErrPtr;
        ipcReplyRegister[endpointQuery.token].absReplyBufferPtr = absQuery.replyPtr;
        ipcReplyRegister[endpointQuery.token].replySizeBytes = endpointQuery.replySizeBytes;
        
        k_proc_lock(procId, PLT_IPC);
    } else {
        endpointQuery.token = MAX_IPC_TOKEN;
    }

    return deliver(listenerPtr, &endpointQuery, false);
}

enum gnwIpcError k_ipc_notify(const struct gnwIpcSenderQuery absQuery,
                              const procId_t target) {

    enum gnwIpcError e = validateNotificationQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.path, absQuery.pathLen, nullptr);
    while (listenerIndex < MAX_IPC_LISTENER) {
        if (ipcListenerRegister[listenerIndex].procId == target) {
            break;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.path, absQuery.pathLen, &listenerIndex);
    }
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NONE;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = KERNEL_PROC_ID;
    endpointQuery.dataPtr = absQuery.dataPtr;
    endpointQuery.dataSizeBytes = absQuery.dataSizeBytes;
    endpointQuery.replySizeBytes = 0;
    endpointQuery.bound = 0;
    endpointQuery.permissions = 0;
    endpointQuery.token = MAX_IPC_TOKEN;

    return deliver(&ipcListenerRegister[listenerIndex], &endpointQuery, true);
}

enum gnwIpcError k_ipc_broadcast(const struct gnwIpcSenderQuery absQuery) {

    enum gnwIpcError e = validateNotificationQuery(absQuery);
    if (e != GIPCE_NONE) {
        return e;
    }

    size_t listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.path, absQuery.pathLen, nullptr);
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return GIPCE_NONE;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = KERNEL_PROC_ID;
    endpointQuery.dataPtr = absQuery.dataPtr;
    endpointQuery.dataSizeBytes = absQuery.dataSizeBytes;
    endpointQuery.replySizeBytes = 0;
    endpointQuery.bound = 0;
    endpointQuery.permissions = 0;
    endpointQuery.token = MAX_IPC_TOKEN;

    while (listenerIndex < MAX_IPC_LISTENER) {
        e = deliver(&ipcListenerRegister[listenerIndex], &endpointQuery, true);
        if (e != GIPCE_NONE) {
            return e;
        }

        listenerIndex = k_ipc_utl_nextListenerIndexForPath(absQuery.path, absQuery.pathLen, &listenerIndex);
    }

    return GIPCE_NONE;
}