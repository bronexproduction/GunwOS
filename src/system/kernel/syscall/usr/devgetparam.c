//
//  devgetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devGetParam(const size_t devId, const size_t param, const size_t paramIndex, size_t * const result) {
    const procId_t procId = k_proc_getCurrentId();
    size_t * absResultPtr = (size_t *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)result, sizeof(size_t));
    if (!absResultPtr) {
        OOPS("Invalid pointer referenced");
    }
    
    return k_dev_getParam(devId, param, paramIndex, result);
}