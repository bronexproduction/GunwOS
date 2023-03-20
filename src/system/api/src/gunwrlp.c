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
    #warning TO BE IMPLEMENTED
}

void runLoopStart() {
    while (1) {
        while (rlp_main.finishedIndex != rlp_main.endIndex) {
            size_t index = ((rlp_main.finishedIndex + 1) % DISPATCH_QUEUE_SIZE);
            struct gnwRunLoopDispatchItem * item = &rlp_main.queue[index];
            
            if (item->format == GEF_NONE) {
                fug(FUG_INCONSISTENT);
            }
            runLoopEntryExecute(item);
            memnull(item, sizeof(struct gnwRunLoopDispatchItem));
            rlp_main.finishedIndex = index;
        }
        
        #warning to be implemented - pause the process and mark as blocked
        continue;
    }
}

ptr_t runLoopGetMain() {
    return (ptr_t)&rlp_main;
}

enum gnwRunLoopError gnwRunLoopDispatch(struct gnwRunLoop * const runLoop, const struct gnwRunLoopDispatchItem dispatchItem) {
    size_t index = ((runLoop->endIndex + 1) % DISPATCH_QUEUE_SIZE);
    if (index == runLoop->finishedIndex) {
        // last empty slot, but let's return error for now
        return GRLE_FULL;
    }
    
    runLoop->queue[index] = dispatchItem;
    runLoop->endIndex = index;
    
    return GRLE_NONE;
}