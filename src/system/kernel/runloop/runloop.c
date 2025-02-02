//
//  runloop.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "runloop.h"
#include <hal/proc/proc.h>
#include <utils.h>
#include <mem.h>
#include <defs.h>
#include <error/panic.h>

#define DISPATCH_QUEUE_SIZE 8
#define DISPATCH_MAX_DATA_SIZE_BYTES KiB(7)
#warning more data may overlap with critical memory region (linker script to be updated?)

#warning critical sections might be useful here instead of in the other layers

struct dispatchItem {
    bool reserved;
    bool handled;
    bool dataHandled;
    struct gnwRunLoopDispatchItem item;
    uint_8 data[DISPATCH_MAX_DATA_SIZE_BYTES];
};

PRIVATE struct runLoop {
    struct dispatchItem queue[DISPATCH_QUEUE_SIZE];
    size_t finishedIndex;
    size_t endIndex;
} rlp_main[MAX_PROC];

static size_t nextIndex(const struct runLoop * const runLoop) {
    return (runLoop->finishedIndex + 1) % DISPATCH_QUEUE_SIZE;
}

static struct dispatchItem * nextDispatchItem(struct runLoop * runLoop, size_t * const index) {
    const size_t queueIndex = nextIndex(runLoop);
    if (index) {
        *index = queueIndex;
    }
    return &runLoop->queue[queueIndex];
}

static struct gnwRunLoopDispatchItem * nextItem(struct runLoop * runLoop) {
    return &nextDispatchItem(runLoop, nullptr)->item;
}

static bool isItemEmpty(const struct gnwRunLoopDispatchItem * const item) {
    return item->format == GEF_NONE;
}

static struct dispatchItem * reservedEmptyItemOrNull(const procId_t procId, const size_t token) {
    struct dispatchItem * const item = &rlp_main[procId].queue[token];
    return (isItemEmpty(&item->item) && item->reserved) ? item : nullptr;
}

static void finishIfNeeded(const procId_t procId, const size_t index) {
    struct runLoop * const loop = &rlp_main[procId];
    struct dispatchItem * const item = nextDispatchItem(loop, nullptr);
    if (!item->handled || !item->dataHandled) {
        return;
    }

    memzero(item, sizeof(struct dispatchItem));
    loop->finishedIndex = index;
}

static enum gnwRunLoopError getPendingDispatchItem(const procId_t procId, struct dispatchItem * * const itemPtr, size_t * const index) {
    if (!IN_RANGE(0, procId, MAX_PROC)) {
        OOPS("Unexpected process ID", GRLE_UNKNOWN);
    }

    struct runLoop * const loop = &rlp_main[procId];
    struct dispatchItem * const item = nextDispatchItem(loop, index);
    if (isItemEmpty(&item->item)) {
        return GRLE_EMPTY;
    }

    *itemPtr = item;
    return GRLE_NONE;
}

enum gnwRunLoopError k_runloop_reserve(const procId_t procId, size_t * const token) {
    if (!IN_RANGE(0, procId, MAX_PROC)) {
        OOPS("Unexpected process ID", GRLE_UNKNOWN);
    }
    if (!token) {
        OOPS("Nullptr", GRLE_UNKNOWN);
    }

    struct runLoop * loop = &rlp_main[procId];
    size_t index = (loop->endIndex + 1) % DISPATCH_QUEUE_SIZE;
    if (loop->queue[index].reserved) {
        return GRLE_FULL;
    }
    
    loop->queue[index].reserved = true;
    loop->endIndex = index;
    
    *token = index;

    return GRLE_NONE;
}

static void release(struct dispatchItem * const item) {
    memzero(item, sizeof(struct dispatchItem));
}

#include <log/log.h>
#include <string.h>

enum gnwRunLoopError k_runloop_dispatch(const procId_t procId,
                                        const size_t token,
                                        const struct gnwRunLoopDispatchItem item,
                                        const ptr_t data,
                                        const gnwRunLoopDataEncodingRoutine dataEncoder) {
    if (!IN_RANGE(0, procId, MAX_PROC)) {
        OOPS("Unexpected process ID", GRLE_UNKNOWN);
    }
    if (token >= DISPATCH_QUEUE_SIZE) {
        OOPS("Unexpected token", GRLE_UNKNOWN);
    }
    struct dispatchItem * const queueItem = reservedEmptyItemOrNull(procId, token);
    if (!queueItem) {
        OOPS("Invalid dispatch item state", GRLE_INVALID_STATE);
    }
    if (item.dataSizeBytes) {
        if (item.dataSizeBytes > DISPATCH_MAX_DATA_SIZE_BYTES) {
            release(queueItem);
            OOPS("Payload too large", GRLE_INVALID_PARAMETER);
        }
        if (!GNWEVENT_ACCEPTS_DATA(item.format)) {
            release(queueItem);
            OOPS("Invalid dispatch format", GRLE_INVALID_PARAMETER);
        }
        if (!data) {
            release(queueItem);
            OOPS("Nullptr", GRLE_INVALID_PARAMETER);
        }
        if (!dataEncoder || !item.decode) {
            release(queueItem);
            OOPS("No encode/decode present", GRLE_INVALID_PARAMETER);
        }
    } else if (data) {
        release(queueItem);
        OOPS("No data expected", GRLE_INVALID_PARAMETER);
    } else {
        queueItem->dataHandled = true;
    }

    queueItem->item = item;
    dataEncoder(data, queueItem->data);
    
    return GRLE_NONE;
}

enum gnwRunLoopError k_runloop_getPendingItem(const procId_t procId, struct gnwRunLoopDispatchItem * const itemPtr) {
    if (!itemPtr) {
        OOPS("Nullptr", GRLE_UNKNOWN);
    }

    struct dispatchItem * item;
    size_t index;
    const enum gnwRunLoopError err = getPendingDispatchItem(procId, &item, &index);
    if (err != GRLE_NONE) {
        return err;
    }
    if (item->handled) {
        return GRLE_INVALID_STATE;
    }

    *itemPtr = item->item;
    item->handled = true;
    finishIfNeeded(procId, index);

    return GRLE_NONE;
}

enum gnwRunLoopError k_runloop_getPendingItemDataSizeBytes(const procId_t procId, size_t * const dataSizeBytes) {
    struct dispatchItem * item;
    size_t index;
    const enum gnwRunLoopError err = getPendingDispatchItem(procId, &item, &index);
    if (err != GRLE_NONE) {
        return err;
    }
    if (!item->handled) {
        return GRLE_INVALID_STATE;
    }
    if (item->dataHandled) {
        return GRLE_INVALID_STATE;
    }
    
    *dataSizeBytes = item->item.dataSizeBytes;
    return GRLE_NONE;
}

enum gnwRunLoopError k_runloop_getPendingItemData(const procId_t procId, ptr_t dataBufferPtr) {
    if (!dataBufferPtr) {
        OOPS("Nullptr", GRLE_UNKNOWN);
    }

    struct dispatchItem * item;
    size_t index;
    const enum gnwRunLoopError err = getPendingDispatchItem(procId, &item, &index);
    if (err != GRLE_NONE) {
        return err;
    }
    if (!item->handled) {
        return GRLE_INVALID_STATE;
    }
    if (item->dataHandled) {
        return GRLE_INVALID_STATE;
    }

    memcopy(item->data, dataBufferPtr, item->item.dataSizeBytes);
    item->dataHandled = true;
    finishIfNeeded(procId, index);
    return GRLE_NONE;
}

bool k_runloop_isEmpty(const procId_t procId) {
    if (!IN_RANGE(0, procId, MAX_PROC)) {
        OOPS("Unexpected process ID", true);
    }

    return isItemEmpty(nextItem(&rlp_main[procId]));
}

void k_runloop_procCleanup(const procId_t procId) {
    if (!IN_RANGE(0, procId, MAX_PROC)) {
        OOPS("Unexpected process ID",);
    }

    memzero(&rlp_main[procId], sizeof(struct runLoop));
}
