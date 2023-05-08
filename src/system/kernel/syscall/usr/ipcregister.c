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
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * desc) {

    const procId_t procId = k_proc_getCurrentId();
    const struct gnwIpcHandlerDescriptor * absDescPtr = (struct gnwIpcHandlerDescriptor *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)desc, sizeof(struct gnwIpcHandlerDescriptor));
    if (!absDescPtr) {
        OOPS("Invalid pointer referenced");
        return GIPCE_UNKNOWN;
    }

    const char * const absPathPtr = (char *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)absDescPtr->path, absDescPtr->pathLen);
    if (!absPathPtr) {
        OOPS("Invalid pointer referenced");
        return GIPCE_UNKNOWN;
    }
    
    return k_ipc_ipcRegister(procId, absPathPtr, absDescPtr->pathLen, absDescPtr->accessScope, absDescPtr->handlerRoutine);
}