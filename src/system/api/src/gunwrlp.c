//
//  gunwrlp.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#include "_gunwrlp.h"
#include <gunwrlp.h>
#include <gunwfug.h>
#include <gunwctrl.h>
#include <mem.h>

static struct gnwRunLoop rlp_main;

void runLoopEntryExecute(const struct gnwRunLoopDispatchItem * const item) {
    switch (item->format) {
    case GEF_U32:
        item->routine._32(item->params[0]);
        break;
    case GEF_U32_U8:
        item->routine._32_8(item->params[0], item->params[1]);
        break;
    case GEF_NONE:
        fug(FUG_INCONSISTENT);
        break;
    }
}

static size_t nextIndex(const struct gnwRunLoop * const runLoop) {
    return (runLoop->finishedIndex + 1) % DISPATCH_QUEUE_SIZE;
}

static struct gnwRunLoopDispatchItem * nextItem(struct gnwRunLoop * const runLoop) {
    return &runLoop->queue[nextIndex(runLoop)];
}

static bool isItemEmpty(const struct gnwRunLoopDispatchItem * const item) {
    return item->format == GEF_NONE;
}

void runLoopStart() {
    while (1) {
        size_t index = nextIndex(&rlp_main);
        struct gnwRunLoopDispatchItem * item = &rlp_main.queue[index];
        
        if (isItemEmpty(item)) {
            waitForEvent();
            continue;
        }
        
        runLoopEntryExecute(item);
        memnull(item, sizeof(struct gnwRunLoopDispatchItem));
        rlp_main.finishedIndex = index;
    }
}

ptr_t runLoopGetMain() {
    return (ptr_t)&rlp_main;
}

enum gnwRunLoopError gnwRunLoopDispatch(struct gnwRunLoop * const runLoop, const struct gnwRunLoopDispatchItem dispatchItem) {
    size_t index = (runLoop->endIndex + 1) % DISPATCH_QUEUE_SIZE;
    if (runLoop->queue[index].format != GEF_NONE) {
        return GRLE_FULL;
    }
    
    runLoop->queue[index] = dispatchItem;
    runLoop->endIndex = index;
    
    return GRLE_NONE;
}

bool gnwRunLoopIsEmpty(struct gnwRunLoop * const runLoop) {
    return isItemEmpty(nextItem(runLoop));
}