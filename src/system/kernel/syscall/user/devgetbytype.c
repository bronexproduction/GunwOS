//
//  devgetbytype.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetByType(const procId_t procId, const enum gnwDeviceType deviceType, struct gnwDeviceUHADesc * const descriptorPtr) {
    MEM_VALIDATE_VPTR(procId, descriptorPtr, struct gnwDeviceUHADesc, GDE_UNKNOWN,);

    return k_dev_getByType(deviceType, descriptorPtr);
}
