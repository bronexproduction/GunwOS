//
//  runloopgetitem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <runloop/runloop.h>

enum gnwRunLoopError k_scr_usr_runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr) {

    const procId_t procId = k_proc_getCurrentId();
    struct gnwRunLoopDispatchItem * const absItemPtr = (struct gnwRunLoopDispatchItem *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)itemPtr, sizeof(struct gnwRunLoopDispatchItem));
    if (!absItemPtr) {
        OOPS("Invalid pointer referenced");
        return GRLE_UNKNOWN;
    }
    
    return k_runloop_getPendingItem(procId, absItemPtr);
}