//
//  gunwuha_keyboard_desc.h
//  Universal Hardware Abstraction for keyboard
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_KEYBOARD_DESC_H
#define GUNWOS_GUNWUHA_KEYBOARD_DESC_H

#include <types.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_KEYBOARD_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_keyboard_desc {
    uint_32 _unused;
};

#endif // GUNWOS_GUNWUHA_KEYBOARD_DESC_H
