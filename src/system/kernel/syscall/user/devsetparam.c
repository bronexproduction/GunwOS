//
//  devsetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devSetParam(const procId_t procId,
                                          const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptorPtr,
                                          const size_t value) {
    if (!paramDescriptorPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)paramDescriptorPtr, sizeof(struct gnwDeviceParamDescriptor))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    
    return k_dev_setParam(procId, devId, *paramDescriptorPtr, value);
}
