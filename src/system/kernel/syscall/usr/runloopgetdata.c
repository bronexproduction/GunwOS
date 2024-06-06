//
//  runloopgetdata.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <hal/proc/proc.h>
#include <runloop/runloop.h>
#include <error/panic.h>

enum gnwRunLoopError k_scr_usr_runLoopGetData(ptr_t dataBufferPtr) {

    if (!dataBufferPtr) {
        OOPS("Unexpected null pointer", GRLE_UNKNOWN);
    }

    const procId_t procId = k_proc_getCurrentId();
    size_t dataSizeBytes;

    enum gnwRunLoopError err = k_runloop_getPendingItemDataSizeBytes(procId, &dataSizeBytes);
    if (err != GRLE_NONE) {
        return err;
    }
    
    return k_runloop_getPendingItemData(procId, dataBufferPtr);
}
