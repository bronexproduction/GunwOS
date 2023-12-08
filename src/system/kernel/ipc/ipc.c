//
//  ipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include "ipc.h"
#include <src/_gunwipc.h>
#include <defs.h>
#include <mem.h>
#include <utils.h>
#include <string.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <queue/queue.h>

#define MAX_IPC_LISTENER 8
#define MAX_IPC_TOKEN 8

static struct ipcListener {
    procId_t procId;
    char path[GNW_PATH_IPC_MAX_LENGTH];
    gnwIpcListener listener;
    gnwIpcEndpointQueryDecoder decoder;
} ipcListenerRegister[MAX_IPC_LISTENER];

static struct ipcReply {
    procId_t senderProcId;
    procId_t handlerProcId;
    enum gnwIpcError * absReplyErrorPtr;
    ptr_t absReplyBufferPtr;
    size_t replySizeBytes;
} ipcReplyRegister[MAX_IPC_TOKEN];

static void clearListener(const size_t entryId) {
    if (entryId >= MAX_IPC_LISTENER) {
        OOPS("Listener index out of bounds");
        return;
    }
    
    memzero(&ipcListenerRegister[entryId], sizeof(struct ipcListener));
    ipcListenerRegister[entryId].procId = NONE_PROC_ID;
}

static void clearReply(const size_t entryId) {
    if (entryId >= MAX_IPC_TOKEN) {
        OOPS("Reply index out of bounds");
        return;
    }
    
    memzero(&ipcReplyRegister[entryId], sizeof(struct ipcReply));
    ipcReplyRegister[entryId].senderProcId = NONE_PROC_ID;
    ipcReplyRegister[entryId].handlerProcId = NONE_PROC_ID;
}

void k_ipc_init() {
    for (size_t i = 0; i < MAX_IPC_LISTENER; clearListener(i++));
    for (size_t i = 0; i < MAX_IPC_TOKEN; clearReply(i++));
}

static bool pathGlobalValidate(const char * absPathPtr, const size_t pathLen) {
    for (size_t i = 0; i < pathLen; ++i) {
        if (IN_RANGE('0', absPathPtr[i], '9')                          ||   /* 0 - 9 */
            IN_RANGE('A', absPathPtr[i], 'Z')                          ||   /* A - Z */
            IN_RANGE('a', absPathPtr[i], 'z')                          ||   /* a - z */
            ((absPathPtr[i] == GNW_PATH_IPC_COMPONENT_SEPARATOR) && i) ||   /* path separator */
            ((absPathPtr[i] == GNW_PATH_IPC_BROADCAST_PREFIX) && !i)        /* broadcast prefix */) {
            continue;
        }

        return false;
    }

    return true;
}

static bool pathIsBroadcast(const char * absPathPtr, const size_t pathLen) {
    return absPathPtr[0] == GNW_PATH_IPC_BROADCAST_PREFIX;
}

static size_t nextListenerIndexForPath(const char * absPathPtr, const size_t pathLen, const size_t * const after) {
    for (size_t index = after ? *after + 1 : 0; index < MAX_IPC_LISTENER; ++index) {
        if (!strcmpl(absPathPtr, ipcListenerRegister[index].path, pathLen)) {
            return index;
        }
    }

    return MAX_IPC_LISTENER;
}

static size_t freeListenerIndex() {
    for (size_t index = 0; index < MAX_IPC_LISTENER; ++index) {
        if (ipcListenerRegister[index].procId == NONE_PROC_ID) {
            return index;
        }
    }
    
    return MAX_IPC_LISTENER;
}

static size_t freeReplyIndex() {
    for (size_t index = 0; index < MAX_IPC_TOKEN; ++index) {
        if (ipcReplyRegister[index].senderProcId == NONE_PROC_ID) {
            return index;
        }
    }

    return MAX_IPC_TOKEN;
}

static bool processPermittedForPath(const procId_t procId,
                                    const char * absPathPtr,
                                    const size_t pathLen) {
    
    bool isKernelBroadcast = false;

    if (pathLen >= 3) {
        /*
            _k/
        */
        if (absPathPtr[0] == GNW_PATH_IPC_BROADCAST_PREFIX &&
            absPathPtr[1] == GNW_PATH_IPC_KERNEL_BROADCAST_ID &&
            absPathPtr[2] == GNW_PATH_IPC_COMPONENT_SEPARATOR) {
            isKernelBroadcast = true;
        }
    }

    return isKernelBroadcast == (procId == KERNEL_PROC_ID);
}

static void unlockIfAble(const procId_t procId) {
    for (size_t token = 0; token < MAX_IPC_TOKEN; ++token) {
        if (ipcReplyRegister[token].senderProcId == procId) {
            return;
        }
    }

    k_proc_unlock(procId, PLT_IPC);
}

enum gnwIpcError k_ipc_send(const procId_t procId,
                            const struct gnwIpcSenderQuery absQuery) {
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

    if (!processPermittedForPath(procId, absQuery.path, absQuery.pathLen)) {
        return GIPCE_FORBIDDEN;
    }

    const bool isBroadcast = pathIsBroadcast(absQuery.path, absQuery.pathLen);
    if ((absQuery.replyPtr || absQuery.replyErrPtr || absQuery.replySizeBytes) && isBroadcast) {
        /*
            Replying to broadcast events not supported
        */
        return GIPCE_INVALID_PARAMETER;
    }

    size_t listenerIndex = nextListenerIndexForPath(absQuery.path, absQuery.pathLen, nullptr);
    if (listenerIndex >= MAX_IPC_LISTENER) {
        return isBroadcast ? GIPCE_NONE : GIPCE_NOT_FOUND;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = procId;
    endpointQuery.dataPtr = absQuery.dataPtr;
    endpointQuery.dataSizeBytes = absQuery.dataSizeBytes;
    endpointQuery.replySizeBytes = absQuery.replySizeBytes;

    size_t token;

    if (endpointQuery.replySizeBytes) {
        if (!absQuery.replyPtr) {
            OOPS("Unexpected null pointer to IPC reply data");
            return GIPCE_UNKNOWN;
        }
        if (!absQuery.replyErrPtr) {
            OOPS("Unexpected null pointer to IPC reply error");
            return GIPCE_UNKNOWN;
        }

        token = freeReplyIndex();
        if (token >= MAX_IPC_TOKEN) {
            OOPS("IPC reply table full");
            return GIPCE_UNKNOWN;
        }

        ipcReplyRegister[token].senderProcId = procId;
        ipcReplyRegister[token].handlerProcId = ipcListenerRegister[listenerIndex].procId;
        ipcReplyRegister[token].absReplyErrorPtr = absQuery.replyErrPtr;
        ipcReplyRegister[token].absReplyBufferPtr = absQuery.replyPtr;
        ipcReplyRegister[token].replySizeBytes = endpointQuery.replySizeBytes;

        endpointQuery.token = token;
        
        k_proc_lock(procId, PLT_IPC);
    } else {
        endpointQuery.token = MAX_IPC_TOKEN;
    }

    while (listenerIndex < MAX_IPC_LISTENER) {
        const enum k_proc_error err = k_proc_callback_invoke_ptr(ipcListenerRegister[listenerIndex].procId,
                                                                 (gnwEventListener_ptr)ipcListenerRegister[listenerIndex].listener,
                                                                 (ptr_t)&endpointQuery,
                                                                 sizeof(struct gnwIpcEndpointQuery) + endpointQuery.dataSizeBytes,
                                                                 sizeof(struct gnwIpcEndpointQuery),
                                                                 (gnwRunLoopDataEncodingRoutine)gnwIpcEndpointQuery_encode,
                                                                 (gnwRunLoopDataEncodingRoutine)ipcListenerRegister[listenerIndex].decoder);

        if (err != PE_NONE) {
            if (endpointQuery.replySizeBytes) {
                clearReply(token);
                k_que_dispatch_arch((fPtr_arch)unlockIfAble, procId);
            }

            if (err == PE_IGNORED && !isBroadcast) {
                return GIPCE_IGNORED;
            } else if (err == PE_LIMIT_REACHED) {
                return GIPCE_FULL;
            } else {
                return GIPCE_UNKNOWN;
            }
        }

        listenerIndex = nextListenerIndexForPath(absQuery.path, absQuery.pathLen, &listenerIndex);
    }

    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_register(const procId_t procId,
                                const char * const absPathPtr,
                                const size_t pathLen,
                                const gnwIpcListener handlerRoutine,
                                const gnwIpcEndpointQueryDecoder decoder) {
    if (!absPathPtr) {
        OOPS("Nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!pathLen || pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    if (!handlerRoutine) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!pathGlobalValidate(absPathPtr, pathLen)) {
        return GIPCE_INVALID_PATH;
    }
    if (!pathIsBroadcast(absPathPtr, pathLen) && nextListenerIndexForPath(absPathPtr, pathLen, nullptr) < MAX_IPC_LISTENER) {
        return GIPCE_ALREADY_EXISTS;
    }

    size_t index = freeListenerIndex();
    if (index >= MAX_IPC_LISTENER) {
        OOPS("IPC listener limit reached");
        return GIPCE_UNKNOWN;
    }

    ipcListenerRegister[index].procId = procId;
    memcopy(absPathPtr, ipcListenerRegister[index].path, pathLen);
    ipcListenerRegister[index].listener = handlerRoutine;
    ipcListenerRegister[index].decoder = decoder;

    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_reply(const procId_t procId,
                             const ptr_t absReplyBufferPtr,
                             const size_t replySizeBytes,
                             const size_t token) {
    if (!absReplyBufferPtr) {
        OOPS("Null reply buffer pointer");
        return GIPCE_UNKNOWN;
    }
    if (!replySizeBytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (token >= MAX_IPC_TOKEN) {
        return GIPCE_INVALID_PARAMETER;
    }

    const struct ipcReply * const reply = &ipcReplyRegister[token];
    if (reply->handlerProcId == NONE_PROC_ID) {
        /*
            Empty slot

            Note: Can happen if sender process terminated before receiving the reply
        */
        return GIPCE_NOT_FOUND;
    }
    if (reply->handlerProcId != procId) {
        return GIPCE_FORBIDDEN;
    }
    if (reply->replySizeBytes != replySizeBytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!reply->absReplyErrorPtr) {
        OOPS("Unexpected reply error nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!reply->absReplyBufferPtr) {
        OOPS("Unexpected reply buffer nullptr");
        return GIPCE_UNKNOWN;
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply");
        return GIPCE_UNKNOWN;
    }

    memcopy(absReplyBufferPtr, reply->absReplyBufferPtr, replySizeBytes);
    *(reply->absReplyErrorPtr) = GIPCE_NONE;

    clearReply(token);
    unlockIfAble(senderProcId);

    return GIPCE_NONE;
}

static void failReply(const size_t token) {
    if (token >= MAX_IPC_TOKEN) {
        OOPS("Unexpected IPC token on error");
        return;
    }

    const struct ipcReply * const reply = &ipcReplyRegister[token];
    if (reply->handlerProcId == NONE_PROC_ID) {
        OOPS("Unexpected empty IPC slot on error");
        return;
    }
    if (!reply->absReplyErrorPtr) {
        OOPS("Unexpected reply error nullptr");
        return;
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply");
        return;
    }

    *(reply->absReplyErrorPtr) = GIPCE_IGNORED;

    clearReply(token);
    unlockIfAble(senderProcId);
}

void k_ipc_procCleanup(const procId_t procId) {
    for (size_t entryId = 0; entryId < MAX_IPC_LISTENER; ++entryId) {
        if (ipcListenerRegister[entryId].procId == procId) {
            clearListener(entryId);
        }
    }
    for (int token = 0; token < MAX_IPC_TOKEN; ++token) {
        if (ipcReplyRegister[token].handlerProcId == procId) {
            /*
                Handler process terminated

                Note: Nullptr reply must be issued and sender process IPC lock must be released
            */
            failReply(token);
        } else if (ipcReplyRegister[token].senderProcId == procId) {
            /*
                Sender process terminated

                Note: Handler process will get GIPCE_NOT_FOUND as ipcReply return value for current token
            */
            clearReply(token);
        }
    }
}
