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
    GRLE_EMPTY,
    GRLE_FULL,
    GRLE_UNKNOWN
};

typedef void (*gnwRunLoopDataSerializationRoutine)(ptr_t, ptr_t);

struct gnwRunLoopDispatchItem {
    enum gnwEventFormat format;
    union gnwEventListener routine;
    size_t dataSizeBytes;
    gnwRunLoopDataSerializationRoutine serialize;
    gnwRunLoopDataSerializationRoutine deserialize;
};

#endif // GUNWOS__GUNWRLP_H
