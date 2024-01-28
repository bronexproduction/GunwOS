//
//  devgetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devGetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptorPtr,
                                          size_t * const resultPtr) {
    const procId_t procId = k_proc_getCurrentId();

    SCLF_GET_VALID_ABS(const struct gnwDeviceParamDescriptor *, paramDescriptorPtr, sizeof(struct gnwDeviceParamDescriptor), {}, GDE_UNKNOWN);
    SCLF_GET_VALID_ABS(size_t *, resultPtr, sizeof(size_t), {}, GDE_UNKNOWN);
    
    return k_dev_getParam(devId, *abs_paramDescriptorPtr, abs_resultPtr);
}