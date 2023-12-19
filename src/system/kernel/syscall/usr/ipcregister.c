//
//  ipcregister.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.04.2023.
//

#include <syscall/func.h>
#include <gunwevent.h>
#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <src/_gunwipc.h>

enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * descPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwIpcHandlerDescriptor *, descPtr, sizeof(struct gnwIpcHandlerDescriptor), {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(const gnwIpcPath, pathPtr, abs_descPtr->path, abs_descPtr->pathLen, {
        return GIPCE_UNKNOWN;
    });
    
    return k_ipc_register(procId,
                          abs_pathPtr,
                          abs_descPtr->pathLen,
                          abs_descPtr->handlerRoutine,
                          abs_descPtr->decoder,
                          abs_descPtr->bindingRequired,
                          abs_descPtr->permissionMask);
}