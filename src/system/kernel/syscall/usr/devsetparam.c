//
//  devsetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#include <hal/proc/proc.h>
#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devSetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                          const size_t value) {
    if (!paramDescriptor) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    
    return k_dev_setParam(k_proc_getCurrentId(), devId, *paramDescriptor, value);
}
