//
//  devmemwrite.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>

enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const ptr_t bufferPtr, const range_addr_t * const devInputRange) {
    const procId_t procId = k_proc_getCurrentId();
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }

    SCLF_GET_VALID_ABS(const range_addr_t *, devInputRange, sizeof(range_addr_t), GDE_UNKNOWN);
    SCLF_GET_VALID_ABS(const ptr_t, bufferPtr, abs_devInputRange->sizeBytes, GDE_UNKNOWN);
    
    return k_dev_writeMem(procId, devId, (ptr_t)abs_bufferPtr, *abs_devInputRange);
}
