//
//  runloopgetitem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include <hal/proc/proc.h>
#include <runloop/runloop.h>
#include <error/panic.h>

enum gnwRunLoopError k_scr_usr_runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr) {

    if (!itemPtr) {
        OOPS("Unexpected null pointer", GRLE_UNKNOWN);
    }
    
    return k_runloop_getPendingItem(k_proc_getCurrentId(), itemPtr);
}