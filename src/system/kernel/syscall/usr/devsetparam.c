//
//  devsetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devSetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                          const size_t value) {
    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwDeviceParamDescriptor *, paramDescriptor, sizeof(struct gnwDeviceParamDescriptor), GDE_UNKNOWN);
    
    return k_dev_setParam(procId, devId, *abs_paramDescriptor, value);
}