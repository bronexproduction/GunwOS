//
//  devmemwrite.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <hal/mem/mem.h>
#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devMemWrite(const procId_t procId, const size_t devId, const ptr_t bufferPtr, const range_addr_t * const devInputRange) {

    if (!bufferPtr) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!devInputRange) {
        OOPS("Unexpected null pointer", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)devInputRange, sizeof(range_addr_t))) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }
    if (!devInputRange->sizeBytes) {
        OOPS("Unexpected buffer size", GDE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)bufferPtr, devInputRange->sizeBytes)) {
        OOPS("Reserved zone access violation", GDE_UNKNOWN);
    }

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }
    
    return k_dev_writeMem(procId, devId, (ptr_t)bufferPtr, *devInputRange);
}
