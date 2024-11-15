//
//  devgetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetParam(const procId_t procId,
                                          const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptorPtr,
                                          size_t * const resultPtr) {
    
    if (!paramDescriptorPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)paramDescriptorPtr, sizeof(struct gnwDeviceParamDescriptor))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    if (!resultPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)resultPtr, sizeof(size_t))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    
    return k_dev_getParam(devId, *paramDescriptorPtr, resultPtr);
}