//
//  ipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include "ipc.h"
#include <mem.h>
#include <utils.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <src/_gunwipc.h>

#define MAX_IPC_LISTENER 16

static struct ipcListener {
    procId_t procId;
    char path[GNW_PATH_IPC_MAX_LENGTH];
    enum gnwIpcAccessScope accessScope;
    gnwIpcListener listener;
} ipcListenerRegister[MAX_IPC_LISTENER];

static void clear(const size_t entryId) {
    if (entryId >= MAX_IPC_LISTENER) {
        OOPS("Listener index out of bounds");
        return;
    }
    
    memzero(&ipcListenerRegister[entryId], sizeof(struct ipcListener));
    ipcListenerRegister[entryId].procId = NONE_PROC_ID;
}

void k_ipc_init() {
    for (size_t i = 0; i < MAX_IPC_LISTENER; clear(i++));
}

static bool pathGlobalValidate(const char * absPathPtr, const size_t pathLen) {
    for (size_t i = 0; i < pathLen; ++i) {
        if (IN_RANGE(48, absPathPtr[i], 57) ||  /* 0 - 9 */
            IN_RANGE(65, absPathPtr[i], 90) ||  /* A - Z */
            IN_RANGE(97, absPathPtr[i], 122)    /* a - z */) {
            continue;
        }

        return false;
    }

    return true;
}

static size_t listenerIndexForPath(const char * absPathPtr, const size_t pathLen, bool * const found) {
    for (size_t index = 0; index < MAX_IPC_LISTENER; ++index) {
        if (!strcmpl(absPathPtr, ipcListenerRegister[index].path, pathLen)) {
            *found = true;
            return index;
        }
    }

    *found = false;
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

static bool processPermitted(const procId_t procId,
                             const enum gnwIpcAccessScope accessScope) {
    return (procId == KERNEL_PROC_ID && (accessScope & GIAS_KERNEL)) ||
           (procId > KERNEL_PROC_ID && (accessScope & GIAS_USER));
}

enum gnwIpcError k_ipc_ipcSend(const procId_t procId,
                               const struct gnwIpcSenderQuery absQuery) {
    if (!absQuery.path || !absQuery.params.dataPtr) {
        OOPS("Nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!absQuery.pathLen || absQuery.pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }

    bool found;
    size_t index = listenerIndexForPath(absQuery.path, absQuery.pathLen, &found);
    if (!found) {
        return GIPCE_NOT_FOUND;
    }
    if (index >= MAX_IPC_LISTENER) {
        OOPS("Listener index out of bounds");
        return GIPCE_UNKNOWN;
    }
    if (!processPermitted(procId, ipcListenerRegister[index].accessScope)) {
        return GIPCE_FORBIDDEN;
    }

    struct gnwIpcEndpointQuery endpointQuery;
    endpointQuery.sourceProcId = procId;
    endpointQuery.params = absQuery.params;

    #warning endpoint query - how to handle response? response bytes not counted
    const enum k_proc_error err = k_proc_callback_invoke_ptr(ipcListenerRegister[index].procId,
                                                             (gnwEventListener_ptr)ipcListenerRegister[index].listener,
                                                             (ptr_t)&endpointQuery,
                                                             sizeof(struct gnwIpcEndpointQuery) + endpointQuery.params.dataBytes,
                                                             (k_proc_callbackDataSerializationRoutine)gnwIpcEndpointQuery_serialize,
                                                             (k_proc_callbackDataSerializationRoutine)gnwIpcEndpointQuery_deserialize);
    if (err == PE_IGNORED) {
        return GIPCE_IGNORED;
    }
    else if (err != PE_NONE) {
        return GIPCE_UNKNOWN;
    }

    return GIPCE_NONE;
}

enum gnwIpcError k_ipc_ipcRegister(const procId_t procId,
                                   const char * const absPathPtr,
                                   const size_t pathLen,
                                   const enum gnwIpcAccessScope accessScope,
                                   const gnwIpcListener handlerRoutine) {
    if (!absPathPtr) {
        OOPS("Nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!pathLen || pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    if (accessScope == GIAS_NONE || accessScope > GIAS_ALL) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!handlerRoutine) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!pathGlobalValidate(absPathPtr, pathLen)) {
        return GIPCE_INVALID_PATH;
    }
    bool registered;
    listenerIndexForPath(absPathPtr, pathLen, &registered);
    if (registered) {
        return GIPCE_ALREADY_EXISTS;
    }

    size_t index = freeListenerIndex();
    if (index >= MAX_IPC_LISTENER) {
        OOPS("IPC listener limit reached");
        return GIPCE_UNKNOWN;
    }

    ipcListenerRegister[index].procId = procId;
    memcopy(absPathPtr, ipcListenerRegister[index].path, pathLen);
    ipcListenerRegister[index].accessScope = accessScope;
    ipcListenerRegister[index].listener = handlerRoutine;

    return GIPCE_NONE;
}

void k_ipc_procCleanup(const procId_t procId) {
    clear(procId);
}
