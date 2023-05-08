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
    size_t dataSizeBytes;
};

#endif // GUNWOS__GUNWRLP_H
