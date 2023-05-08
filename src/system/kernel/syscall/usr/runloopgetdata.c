//
//  runloopgetdata.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <hal/proc/proc.h>
#include <runloop/runloop.h>
#include <syscall/func.h>
#include <error/panic.h>

enum gnwRunLoopError k_scr_usr_runLoopGetData(ptr_t dataBufferPtr) {

    const procId_t procId = k_proc_getCurrentId();
    size_t dataSizeBytes;

    enum gnwRunLoopError err = k_runloop_getPendingItemDataSizeBytes(procId, &dataSizeBytes);
    if (err != GRLE_NONE) {
        return err;
    }

    ptr_t absDataBufferPtr = k_scl_func_getValidAbsoluteForProc(procId, dataBufferPtr, dataSizeBytes);
    if (!absDataBufferPtr) {
        OOPS("Invalid pointer referenced");
        return GRLE_UNKNOWN;
    }
    
    return k_runloop_getPendingItemData(procId, absDataBufferPtr);
}
