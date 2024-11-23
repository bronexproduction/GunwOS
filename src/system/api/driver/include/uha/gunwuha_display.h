//
//  gunwuha_display.h
//  Universal Hardware Abstraction for a display
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.
//

#ifndef GUNWOS_GUNWUHA_DISPLAY_H
#define GUNWOS_GUNWUHA_DISPLAY_H

#include <uha/gunwuha_display_desc.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_DISPLAY GNW_UHA_EMPTY(DISPLAY)
#define GNW_UHA_DISPLAY_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_display_routine {
    void (*_unused);
};

struct gnwDeviceUHA_display {
    struct gnwDeviceUHA_display_desc desc;
    struct gnwDeviceUHA_display_routine routine;
};

#endif // GUNWOS_GUNWUHA_DISPLAY_H
