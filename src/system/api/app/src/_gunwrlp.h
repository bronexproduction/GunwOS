//
//  _gunwrlp.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS__GUNWRLP_H
#define GUNWOS__GUNWRLP_H

#include "../include/gunwevent.h"

enum gnwRunLoopError {
    GRLE_NONE = 0,
    GRLE_EMPTY,
    GRLE_FULL,
    GRLE_INVALID_STATE,
    GRLE_INVALID_PARAMETER,
    GRLE_UNKNOWN
};

typedef void (*gnwRunLoopDataEncodingRoutine)(ptr_t source, ptr_t destination);

struct gnwRunLoopDispatchItem {
    enum gnwEventFormat format;
    union gnwEventListener routine;
    size_t dataSizeBytes;
    size_t decodedDataSizeBytes;
    gnwRunLoopDataEncodingRoutine decode;
};

#endif // GUNWOS__GUNWRLP_H
