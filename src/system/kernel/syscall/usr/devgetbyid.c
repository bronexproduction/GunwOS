//
//  devgetbyid.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetById(const size_t deviceId, struct gnwDeviceUHADesc * const descriptorPtr) {
    
    if (!descriptorPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }

    return k_dev_getById(deviceId, descriptorPtr);
}
