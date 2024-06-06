//
//  registration.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "ipc.h"
#include "data.h"
#include "utility.h"
#include <src/_gunwipc.h>
#include <defs.h>
#include <mem.h>
#include <hal/proc/proc.h>
#include <string.h>
#include <error/panic.h>

static size_t freeListenerIndex() {
    for (size_t index = 0; index < MAX_IPC_LISTENER; ++index) {
        if (ipcListenerRegister[index].procId == NONE_PROC_ID) {
            return index;
        }
    }
    
    return MAX_IPC_LISTENER;
}

static bool receivesKernelNotifications(const procId_t procId) {
    for (size_t i = 0; i < MAX_IPC_LISTENER; ++i) {
        if (ipcListenerRegister[i].procId == procId &&
            (ipcListenerRegister[i].path ? !strcmp(ipcListenerRegister[i].path, GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED) : false)) {
            return true;
        }
    }

    return false;
}

enum gnwIpcError k_ipc_register(const procId_t procId,
                                const data_t absPathData,
                                const gnwIpcListener handlerRoutine,
                                const gnwIpcEndpointQueryDecoder decoder,
                                const bool bindingRequired,
                                const size_t permissionMask) {
    if (procId <= KERNEL_PROC_ID) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!absPathData.ptr) {
        OOPS("Nullptr", GIPCE_UNKNOWN);
    }
    if (!absPathData.bytes || absPathData.bytes > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    if (!handlerRoutine) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!decoder) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!bindingRequired && permissionMask) {
        return GIPCE_INVALID_PARAMETER;
    }

    enum gnwIpcListenerType type = k_ipc_utl_pathGlobalValidate(absPathData) ? GILT_GLOBAL : GILT_NONE;
    type |= k_ipc_utl_pathDirectValidate(absPathData) ? GILT_DIRECT : GILT_NONE;
    type |= k_ipc_utl_pathNotificationValidate(absPathData) ? GILT_NOTIFICATION : GILT_NONE;

    if (__builtin_popcount(type) != 1) {
        return GIPCE_INVALID_PATH;
    }

    /*
        Binding parameters validation
    */

    if ((type == GILT_DIRECT) && !bindingRequired) {
        return GIPCE_INVALID_PARAMETER;
    }
    if ((type == GILT_NOTIFICATION) && bindingRequired) {
        return GIPCE_INVALID_PARAMETER;
    }

    /*
        Check binding preconditions
    */

    if (bindingRequired && !receivesKernelNotifications(procId)) {
        return GIPCE_PRECONDITION_NOT_SATISFIED;
    }

    /*
        Check if global listener already exists
    */

    if ((type == GILT_GLOBAL) && k_ipc_utl_nextListenerIndexForPath(absPathData, nullptr) < MAX_IPC_LISTENER) {
        return GIPCE_ALREADY_EXISTS;
    }

    size_t index = freeListenerIndex();
    if (index >= MAX_IPC_LISTENER) {
        OOPS("IPC listener limit reached", GIPCE_UNKNOWN);
    }

    ipcListenerRegister[index].procId = procId;
    memcopy(absPathData.ptr, ipcListenerRegister[index].path, absPathData.bytes);
    ipcListenerRegister[index].type = type;
    ipcListenerRegister[index].listener = handlerRoutine;
    ipcListenerRegister[index].decoder = decoder;
    ipcListenerRegister[index].bindingRequired = bindingRequired;
    ipcListenerRegister[index].permissionMask = permissionMask;

    return GIPCE_NONE;
}
