//
//  gunwuha_keyboard.h
//  Universal Hardware Abstraction for keyboard
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_KEYBOARD_H
#define GUNWOS_GUNWUHA_KEYBOARD_H

#include <stdgunw/types.h>

struct gnwDeviceUHA_keyboard_desc {
    uint_32 _unused;
};

struct gnwDeviceUHA_keyboard_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_keyboard {
    struct gnwDeviceUHA_keyboard_desc desc;
    struct gnwDeviceUHA_keyboard_routine routine;
};

#endif // GUNWOS_GUNWUHA_KEYBOARD_H