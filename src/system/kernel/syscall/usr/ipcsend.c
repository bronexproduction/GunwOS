//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <syscall/func.h>
#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcSend(const char * const path, const size_t pathLen, const char c) {
    const procId_t procId = k_proc_getCurrentId();
    const ptr_t absPathPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)path, pathLen);
    if (!absPathPtr) {
        OOPS("Invalid pointer referenced");
    }
    
    return k_ipc_ipcSend(procId, (char *)absPathPtr, pathLen, c);
}