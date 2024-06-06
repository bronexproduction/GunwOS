//
//  devmemwrite.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <hal/proc/proc.h>
#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const ptr_t bufferPtr, const range_addr_t * const devInputRange) {

    if (!bufferPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!devInputRange) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!devInputRange->sizeBytes) {
        OOPS("Unexpected buffer size", GDE_UNKNOWN);
    }

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }
    
    return k_dev_writeMem(k_proc_getCurrentId(), devId, (ptr_t)bufferPtr, *devInputRange);
}
