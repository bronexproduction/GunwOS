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

enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const void * const buf) {
    const procId_t procId = k_proc_getCurrentId();
    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError err = k_dev_getById(devId, &desc);
    if (err != GDE_NONE) {
        return err;
    }
    if (!desc.mem.inputSizeBytes) {
        return GDE_INVALID_OPERATION;
    }

    const ptr_t absBufferPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)buf, desc.mem.inputSizeBytes);
    if (!absBufferPtr) {
        OOPS("Null pointer referenced");
    }
    
    return k_dev_writeMem((const size_t)k_proc_getCurrentId(), (const size_t)devId, (const void * const)absBufferPtr);
}
