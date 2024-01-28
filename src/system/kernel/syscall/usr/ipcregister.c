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
    SCLF_GET_VALID_ABS(const struct gnwIpcHandlerDescriptor *, descPtr, sizeof(struct gnwIpcHandlerDescriptor), {}, GIPCE_UNKNOWN);
    SCLF_GET_VALID_ABS_NAMED(const gnwIpcPath, pathPtr, abs_descPtr->pathData.ptr, abs_descPtr->pathData.bytes, {}, GIPCE_UNKNOWN);
    
    data_t absPathData;
    absPathData.ptr = (ptr_t)abs_pathPtr;
    absPathData.bytes = abs_descPtr->pathData.bytes;

    return k_ipc_register(procId,
                          absPathData,
                          abs_descPtr->handlerRoutine,
                          abs_descPtr->decoder,
                          abs_descPtr->bindingRequired,
                          abs_descPtr->permissionMask);
}