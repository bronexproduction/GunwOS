//
//  devgetbyid.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <dev/dev.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devGetById(const procId_t procId, const size_t deviceId, struct gnwDeviceUHADesc * const descriptorPtr) {
    MEM_VALIDATE_VPTR(procId, descriptorPtr, struct gnwDeviceUHADesc,
        { OOPS("Unexpected null pointer", GDE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GDE_UNKNOWN); }
    )

    return k_dev_getById(deviceId, descriptorPtr);
}
