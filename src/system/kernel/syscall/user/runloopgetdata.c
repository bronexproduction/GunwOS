//
//  runloopgetdata.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <runloop/runloop.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwRunLoopError k_scr_usr_runLoopGetData(const procId_t procId, ptr_t dataBufferPtr) {

    if (!dataBufferPtr) {
        OOPS("Unexpected null pointer", GRLE_UNKNOWN);
    }

    size_t dataSizeBytes;

    enum gnwRunLoopError err = k_runloop_getPendingItemDataSizeBytes(procId, &dataSizeBytes);
    if (err != GRLE_NONE) {
        return err;
    }

    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)dataBufferPtr, dataSizeBytes)) {
        OOPS("Reserved zone access violation", GRLE_UNKNOWN);
    }
    
    return k_runloop_getPendingItemData(procId, dataBufferPtr);
}
