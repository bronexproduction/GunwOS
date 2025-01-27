//
//  gunwuha_keyboard.h
//  Universal Hardware Abstraction for keyboard
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_KEYBOARD_H
#define GUNWOS_GUNWUHA_KEYBOARD_H

#include <uha/gunwuha_keyboard_desc.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_KEYBOARD GNW_UHA_EMPTY(KEYBOARD)
#define GNW_UHA_KEYBOARD_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_keyboard_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_keyboard {
    struct gnwDeviceUHA_keyboard_desc desc;
    struct gnwDeviceUHA_keyboard_routine routine;
};

#endif // GUNWOS_GUNWUHA_KEYBOARD_H