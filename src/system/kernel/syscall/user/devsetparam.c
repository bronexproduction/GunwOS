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
                                          const struct gnwDeviceSetParamQuery * const vParamQueryPtr,
                                          const size_t value) {
    if (!vParamQueryPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vParamQueryPtr, sizeof(struct gnwDeviceSetParamQuery))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    
    return k_dev_setParam(procId, devId, *vParamQueryPtr, value);
}
