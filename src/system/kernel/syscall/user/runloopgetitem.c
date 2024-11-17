//
//  runloopgetitem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <runloop/runloop.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwRunLoopError k_scr_usr_runLoopGetItem(const procId_t procId, struct gnwRunLoopDispatchItem * const itemPtr) {

    if (!itemPtr) {
        OOPS("Unexpected null pointer", GRLE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)itemPtr, sizeof(struct gnwRunLoopDispatchItem))) {
        OOPS("Reserved zone access violation", GRLE_UNKNOWN);
    }
    
    return k_runloop_getPendingItem(procId, itemPtr);
}