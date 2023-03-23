//
//  _gunwrlp.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS__GUNWRLP_H
#define GUNWOS__GUNWRLP_H

#include <gunwevent.h>

enum gnwRunLoopError {
    GRLE_NONE = 0,
    GRLE_FULL
};

struct gnwRunLoopDispatchItem {
    enum gnwEventFormat format;
    union gnwEventListener routine;
    uint_32 params[2];
};

#define DISPATCH_QUEUE_SIZE 10

struct gnwRunLoop {
    struct gnwRunLoopDispatchItem queue[DISPATCH_QUEUE_SIZE];
    size_t finishedIndex;
    size_t endIndex;
};

enum gnwRunLoopError gnwRunLoopDispatch(struct gnwRunLoop * const runLoop, const struct gnwRunLoopDispatchItem dispatchItem);

bool gnwRunLoopIsEmpty(struct gnwRunLoop * const runLoop);

#endif // GUNWOS__GUNWRLP_H
