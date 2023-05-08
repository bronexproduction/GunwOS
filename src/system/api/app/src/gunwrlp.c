//
//  gunwrlp.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "_gunwrlp.h"
#include <defs.h>
#include <gunwfug.h>

/*
    Asks system for the current run loop entry
*/
SYSCALL_DECL bool runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr) {
    CHECKPTR(itemPtr);

    SYSCALL_PAR1(itemPtr);

    SYSCALL_USER_FUNC(RUNLOOP_GET_ITEM);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(8);
}

/*
    Asks system for the data associated with the current run loop entry
*/
SYSCALL_DECL enum gnwRunLoopError runLoopGetData(ptr_t dataBufferPtr) {
    CHECKPTR(dataBufferPtr);

    SYSCALL_PAR1(dataBufferPtr);

    SYSCALL_USER_FUNC(RUNLOOP_GET_DATA);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

static void execute(const union gnwEventListener routine, const ptr_t data) {
    data ? routine._ptr(data) : routine._void();
}

void runLoopStart() {
    struct gnwRunLoopDispatchItem currentItem;
    while (1) {
        if (!runLoopGetItem(&currentItem)) {
            waitForEvent();
            continue;
        }
        if (currentItem.format == GEF_PTR) {
            uint_8 data[currentItem.dataSizeBytes];
            enum gnwRunLoopError err = runLoopGetData(data);
            if (err != GRLE_NONE) {
                fug(FUG_INCONSISTENT);
            }
            execute(currentItem.routine, data);
        } else {
            execute(currentItem.routine, nullptr);
        }
    }
}

#endif // _GUNWAPI_KERNEL
