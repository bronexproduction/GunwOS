//
//  devgetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.
//

#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetParam(const size_t devId, 
                                          const struct gnwDeviceParamDescriptor * const paramDescriptorPtr,
                                          size_t * const resultPtr) {
    
    if (!paramDescriptorPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!resultPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    
    return k_dev_getParam(devId, *paramDescriptorPtr, resultPtr);
}