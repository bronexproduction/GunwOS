//
//  devmemwrite.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>
#include <error/panic.h>

enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const void * const buf, const range_addr_t * const devInputRange) {
    const procId_t procId = k_proc_getCurrentId();
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }

    const range_addr_t * const absDevInputRangePtr = (range_addr_t *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)devInputRange, sizeof(range_addr_t));
    if (!absDevInputRangePtr) {
        OOPS("Invalid pointer referenced");
        return GDE_UNKNOWN;
    }

    const ptr_t absBufferPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)buf, absDevInputRangePtr->sizeBytes);
    if (!absBufferPtr) {
        OOPS("Invalid pointer referenced");
        return GDE_UNKNOWN;
    }
    
    return k_dev_writeMem(k_proc_getCurrentId(), devId, (const void * const)absBufferPtr, *absDevInputRangePtr);
}
