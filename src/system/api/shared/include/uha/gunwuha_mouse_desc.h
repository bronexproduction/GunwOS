//
//  gunwuha_mouse_desc.h
//  Universal Hardware Abstraction for mouse
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#ifndef GUNWOS_GUNWUHA_MOUSE_DESC_H
#define GUNWOS_GUNWUHA_MOUSE_DESC_H

#include <types.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_MOUSE_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_mouse_desc {
    uint_32 _unused;
};

#endif // GUNWOS_GUNWUHA_MOUSE_DESC_H
