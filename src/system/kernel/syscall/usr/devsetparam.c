//
//  devsetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devSetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                          const size_t value) {
    const procId_t procId = k_proc_getCurrentId();

    const struct gnwDeviceParamDescriptor * const absDescPtr = (struct gnwDeviceParamDescriptor *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)paramDescriptor, sizeof(struct gnwDeviceParamDescriptor));
    if (!absDescPtr) {
        OOPS("Invalid pointer referenced");
        return GDE_UNKNOWN;
    }
    
    return k_dev_setParam(procId, devId, *absDescPtr, value);
}