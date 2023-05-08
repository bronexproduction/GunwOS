//
//  runloopgetdata.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <runloop/runloop.h>

enum gnwRunLoopError k_scr_usr_runLoopGetData(ptr_t dataBufferPtr) {

    const procId_t procId = k_proc_getCurrentId();
    const size_t dataSizeBytes;

    enum gnwRunLoopError err = k_runloop_getPendingItemDataBytes(procId, &dataSizeBytes);
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
