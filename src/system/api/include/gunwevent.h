//
//  gunwevent.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS_GUNWEVENT_H
#define GUNWOS_GUNWEVENT_H

#include <types.h>

enum gnwEventFormat {
    GEF_NONE = 0,
    GEF_U32,
    GEF_U32_U8
};

typedef __attribute__((cdecl)) void (*gnwEventListener_32)(int_32 p0);
typedef __attribute__((cdecl)) void (*gnwEventListener_32_8)(int_32 p0, int_8 p1);

union gnwEventListener {
    uint_32 _handle;
    gnwEventListener_32 _32;
    gnwEventListener_32_8 _32_8;
};
_Static_assert(sizeof(union gnwEventListener) == sizeof(int_32), "Unexpected union gnwEventListener size");

#endif // GUNWOS_GUNWEVENT_H
