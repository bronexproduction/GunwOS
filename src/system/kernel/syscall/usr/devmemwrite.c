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

enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const void * const buf, const range_addr_t * const devMemRange) {
    const procId_t procId = k_proc_getCurrentId();
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }

    const range_addr_t * const absDevMemRangePtr = (range_addr_t *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)devMemRange, sizeof(range_addr_t));
    if (!absDevMemRangePtr) {
        OOPS("Invalid pointer referenced");
    }

    const ptr_t absBufferPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)buf, absDevMemRangePtr->sizeBytes);
    if (!absBufferPtr) {
        OOPS("Invalid pointer referenced");
    }
    
    return k_dev_writeMem(k_proc_getCurrentId(), devId, (const void * const)absBufferPtr, *absDevMemRangePtr);
}
