//
//  gunwevent.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS_GUNWEVENT_H
#define GUNWOS_GUNWEVENT_H

enum gnwEventFormat {
    GEF_NONE = 0,
    GEF_VOID,
    GEF_U8
};

typedef __attribute__((cdecl)) void (*gnwEventListener_void)(int_32 type);
typedef __attribute__((cdecl)) void (*gnwEventListener_u8)(int_32 type, uint_8 data);

union gnwEventListener {
    uint_32 _handle;
    gnwEventListener_void onEvent_void;
    gnwEventListener_u8 onEvent_u8;
};
_Static_assert(sizeof(union gnwEventListener) == sizeof(uint_32), "Unexpected union gnwEventListener size");

#endif // GUNWOS_GUNWEVENT_H
