//
//  devgetbytype.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devGetByType(const enum gnwDeviceType deviceType, struct gnwDeviceUHADesc * const descriptorPtr) {
    const procId_t procId = k_proc_getCurrentId();
    
    SCLF_GET_VALID_ABS(const ptr_t, descriptorPtr, sizeof(struct gnwDeviceUHADesc), GDE_UNKNOWN);

    return k_dev_getByType(deviceType, (struct gnwDeviceUHADesc * const)abs_descriptorPtr);
}
