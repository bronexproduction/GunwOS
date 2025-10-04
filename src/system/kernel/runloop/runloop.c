//
//  runloop.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "runloop.h"
#include <hal/proc/proc.h>
#include <objects/objects.h>
#include <utils.h>
#include <mem.h>
#include <defs.h>
#include <error/panic.h>

#define DISPATCH_QUEUE_SIZE 10
#define DISPATCH_MAX_DATA_SIZE_BYTES KiB(7)

#warning critical sections might be useful here instead of in the other layers

struct dispatchItem {
    bool reserved;
    bool handled;
    bool dataHandled;
    struct gnwRunLoopDispatchItem item;
    k_obj_handle dataHandle;
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
    // TODO: race condition in multi-core environment?
    struct dispatchItem * const item = &rlp_main[procId].queue[token];
    return (isItemEmpty(&item->item) && item->reserved) ? item : nullptr;
}

static void release(const procId_t procId, struct dispatchItem * const item) {
    k_obj_remove(procId, item->dataHandle);
    memzero(item, sizeof(struct dispatchItem));
}

static void finishIfNeeded(const procId_t procId, const size_t index) {
    struct runLoop * const loop = &rlp_main[procId];
    struct dispatchItem * const item = nextDispatchItem(loop, nullptr);
    if (!item->handled || !item->dataHandled) {
        return;
    }

    release(procId, item);
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
            release(procId, queueItem);
            OOPS("Payload too large", GRLE_INVALID_PARAMETER);
        }
        if (!GNWEVENT_ACCEPTS_DATA(item.format)) {
            release(procId, queueItem);
            OOPS("Invalid dispatch format", GRLE_INVALID_PARAMETER);
        }
        if (!data) {
            release(procId, queueItem);
            OOPS("Nullptr", GRLE_INVALID_PARAMETER);
        }
        if (!dataEncoder || !item.decode) {
            release(procId, queueItem);
            OOPS("No encode/decode present", GRLE_INVALID_PARAMETER);
        }
    } else if (data) {
        release(procId, queueItem);
        OOPS("No data expected", GRLE_INVALID_PARAMETER);
    } else {
        queueItem->dataHandled = true;
    }

    queueItem->item = item;
    if (data) {
        byte_t buffer[queueItem->item.dataSizeBytes];
        dataEncoder(data, buffer);
        const enum k_obj_error objError = k_obj_store(procId, queueItem->item.dataSizeBytes, buffer, &(queueItem->dataHandle));
        if (objError == OE_SIZE_LIMIT_EXCEEDED) {
            release(procId, queueItem);
            OOPS("Payload too large for object storage", GRLE_INVALID_PARAMETER);
        } else if (objError == OE_FULL) {
            release(procId, queueItem);
            OOPS("Object storage full", GRLE_UNKNOWN);
        } else if (objError != OE_NONE) {
            release(procId, queueItem);
            OOPS("Unknown error", GRLE_UNKNOWN);
        }
    }
    
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

    const enum k_obj_error objErr = k_obj_retrieve(procId, item->dataHandle, item->item.dataSizeBytes, dataBufferPtr);
    if (objErr != OE_NONE) {
        return GRLE_INTERNAL_INCONSISTENCY;
    }
    
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
