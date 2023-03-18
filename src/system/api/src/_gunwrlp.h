//
//  _gunwrlp.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS__GUNWRLP_H
#define GUNWOS__GUNWRLP_H

#include <gunwevent.h>

struct gnwRunLoopDispatchItem {
    enum gnwEventFormat format;
    union gnwEventListener routine;
};

#define DISPATCH_QUEUE_SIZE 10

struct gnwRunLoop {
    struct gnwRunLoopDispatchItem queue[DISPATCH_QUEUE_SIZE];
};

#endif // GUNWOS__GUNWRLP_H
