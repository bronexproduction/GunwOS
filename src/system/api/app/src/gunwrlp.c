//
//  gunwrlp.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "_gunwrlp.h"
#include <defs.h>
#include "../include/gunwctrl.h"
#include "../include/gunwfug.h"
#include "../include/gunwrlp.h"
#include "scl_user.h"

/*
    Asks system for the current run loop entry
*/
static enum gnwRunLoopError runLoopGetItem(struct gnwRunLoopDispatchItem * const itemPtr) {
    CHECKPTR(itemPtr);

    SYSCALL_USER_CALL(RUNLOOP_GET_ITEM, itemPtr, 0, 0);

    return SYSCALL_RESULT;
}

/*
    Asks system for the data associated with the current run loop entry
*/
static enum gnwRunLoopError runLoopGetData(ptr_t dataBufferPtr) {
    CHECKPTR(dataBufferPtr);

    SYSCALL_USER_CALL(RUNLOOP_GET_DATA, dataBufferPtr, 0, 0);

    return SYSCALL_RESULT;
}

static void execute(const union gnwEventListener routine, const ptr_t data) {
    data ? routine._ptr(data) : routine._void();
}

void runLoopStart() {
    while (1) {
        runLoopHandle();
        waitForEvent();
    }
}

void runLoopHandle() {
    struct gnwRunLoopDispatchItem currentItem;
    while (1) {
        const enum gnwRunLoopError err = runLoopGetItem(&currentItem);
        if (err == GRLE_EMPTY) {
            return;
        } else if (err != GRLE_NONE) {
            fug(FUG_INCONSISTENT);
            return;
        }

        if (currentItem.format == GEF_PTR) {
            uint_8 data[currentItem.dataSizeBytes];
            enum gnwRunLoopError err = runLoopGetData(data);
            if (err != GRLE_NONE) {
                fug(FUG_INCONSISTENT);
            }

            uint_8 decodedData[currentItem.decodedDataSizeBytes];
            currentItem.decode(data, decodedData);

            execute(currentItem.routine, decodedData);
        } else {
            execute(currentItem.routine, nullptr);
        }
    }
}

#endif // _GUNWAPI_KERNEL
