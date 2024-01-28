//
//  devgetbyid.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devGetById(const size_t deviceId, struct gnwDeviceUHADesc * const descriptorPtr) {
    const procId_t procId = k_proc_getCurrentId();
    
    SCLF_GET_VALID_ABS(struct gnwDeviceUHADesc * const, descriptorPtr, sizeof(struct gnwDeviceUHADesc), {}, GDE_UNKNOWN);

    return k_dev_getById(deviceId, abs_descriptorPtr);
}
