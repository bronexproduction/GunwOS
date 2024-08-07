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

#include <gunwlog.h>
#include <string.h>

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

            {
                char msg[14] = "runLoopHandle";
                log(msg);
            }
            {
                char msg[128] = "  data buffer pointer -         ";
                uint2hex((addr_t)data, msg + 24);
                log(msg);
            }
            {
                char msg[128] = "  data size bytes -         ";
                uint2dec((addr_t)currentItem.dataSizeBytes, msg + 33);
                log(msg);
            }
            {
                char msg[19] = "  item data bytes:";
                log(msg);
                for (size_t i = 0; i < currentItem.dataSizeBytes; ++i) {
                    char byteString[3] = { 0 };
                    uint2hex((addr_t)((uint_8 *)data)[i], byteString);
                    log(byteString);
                }
            }

            uint_8 decodedData[currentItem.decodedDataSizeBytes];
            currentItem.decode(data, decodedData);

            {
                char msg[128] = "  decoded data buffer pointer -         ";
                uint2hex((addr_t)data, msg + 32);
                log(msg);
            }
            {
                char msg[128] = "  decoded data size bytes -         ";
                uint2dec((addr_t)currentItem.decodedDataSizeBytes, msg + 28);
                log(msg);
            }
            {
                char msg[27] = "  item decoded data bytes:";
                log(msg);
                for (size_t i = 0; i < currentItem.decodedDataSizeBytes; ++i) {
                    char byteString[3] = { 0 };
                    uint2hex((addr_t)((uint_8 *)decodedData)[i], byteString);
                    log(byteString);
                }
            }

            execute(currentItem.routine, decodedData);
        } else {
            execute(currentItem.routine, nullptr);
        }
    }
}

#endif // _GUNWAPI_KERNEL
