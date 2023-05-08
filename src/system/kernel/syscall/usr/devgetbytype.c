//
//  devgetbytype.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <syscall/func.h>
#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetByType(const enum gnwDeviceType deviceType, struct gnwDeviceUHADesc * const descriptor) {
    const ptr_t absDescriptorPtr = k_scl_func_getValidAbsoluteForCurrentProc((const ptr_t)descriptor, sizeof(struct gnwDeviceUHADesc));
    if (!absDescriptorPtr) {
        OOPS("Invalid parameter");
        return GDE_UNKNOWN;
    }

    return k_dev_getByType(deviceType, (struct gnwDeviceUHADesc * const)absDescriptorPtr);
}
