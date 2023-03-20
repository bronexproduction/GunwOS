//
//  gunwrlp.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#include "_gunwrlp.h"
#include <gunwrlp.h>
#include <gunwfug.h>
#include <defs.h>
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

void runLoopStart() {
    for (int i = 0; i < DISPATCH_QUEUE_SIZE; ++i) {
        if (rlp_main.queue[i].format != GEF_NONE) {
            // BSS section not cleared?
            fug(0x69);
        }
    }
    while (1) {
        size_t index = ((rlp_main.finishedIndex + 1) % DISPATCH_QUEUE_SIZE);
        struct gnwRunLoopDispatchItem * item = &rlp_main.queue[index];
        
        if (item->format == GEF_NONE) {
            #warning to be implemented - pause the process and mark as blocked
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
