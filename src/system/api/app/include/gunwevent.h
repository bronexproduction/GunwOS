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
    GEF_VOID,
    GEF_PTR
};

#define GNWEVENT_ACCEPTS_DATA(FORMAT) (FORMAT == GEF_PTR)

typedef __attribute__((cdecl)) void (*gnwEventListener_void)();
typedef __attribute__((cdecl)) void (*gnwEventListener_ptr)(ptr_t p0);

union gnwEventListener {
    uint_32 _handle;
    gnwEventListener_void _void;
    gnwEventListener_ptr _ptr;
};
_Static_assert(sizeof(union gnwEventListener) == sizeof(int_32), "Unexpected union gnwEventListener size");

#endif // GUNWOS_GUNWEVENT_H
