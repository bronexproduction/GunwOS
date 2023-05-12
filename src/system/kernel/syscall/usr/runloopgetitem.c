//
//  runloopgetitem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <syscall/func.h>
#include <hal/proc/proc.h>
#include <runloop/runloop.h>

enum gnwRunLoopError k_scr_usr_runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(struct gnwRunLoopDispatchItem * const, itemPtr, sizeof(struct gnwRunLoopDispatchItem), {
        return GRLE_UNKNOWN;
    });
    
    return k_runloop_getPendingItem(procId, abs_itemPtr);
}