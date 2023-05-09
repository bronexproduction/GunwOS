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

enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * descPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwIpcHandlerDescriptor *, descPtr, sizeof(struct gnwIpcHandlerDescriptor), GIPCE_UNKNOWN);
    SCLF_GET_VALID_ABS_NAMED(const char * const, pathPtr, abs_descPtr->path, abs_descPtr->pathLen, GIPCE_UNKNOWN);
    
    return k_ipc_ipcRegister(procId, abs_pathPtr, abs_descPtr->pathLen, abs_descPtr->accessScope, abs_descPtr->handlerRoutine);
}