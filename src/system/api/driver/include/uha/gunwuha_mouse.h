//
//  gunwuha_mouse.h
//  Universal Hardware Abstraction for mouse
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#ifndef GUNWOS_GUNWUHA_MOUSE_H
#define GUNWOS_GUNWUHA_MOUSE_H

#include <uha/gunwuha_mouse_desc.h>

struct gnwDeviceUHA_mouse_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_mouse {
    struct gnwDeviceUHA_mouse_desc desc;
    struct gnwDeviceUHA_mouse_routine routine;
};

#endif // GUNWOS_GUNWUHA_MOUSE_H