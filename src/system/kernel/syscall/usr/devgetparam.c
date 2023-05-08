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

enum gnwDeviceError k_scr_usr_devGetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                          size_t * const result) {
    const procId_t procId = k_proc_getCurrentId();

    const struct gnwDeviceParamDescriptor * const absDescPtr = (struct gnwDeviceParamDescriptor *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)paramDescriptor, sizeof(struct gnwDeviceParamDescriptor));
    if (!absDescPtr) {
        OOPS("Invalid pointer referenced");
        return GDE_UNKNOWN;
    }

    size_t * absResultPtr = (size_t *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)result, sizeof(size_t));
    if (!absResultPtr) {
        OOPS("Invalid pointer referenced");
        return GDE_UNKNOWN;
    }
    
    return k_dev_getParam(devId, *absDescPtr, absResultPtr);
}