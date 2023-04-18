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

struct gnwDeviceUHA_display_routine {
    /*
        Enables the display
    */
    void (*enable)();
};

struct gnwDeviceUHA_display {
    struct gnwDeviceUHA_display_desc desc;
    struct gnwDeviceUHA_display_routine routine;
};

#endif // GUNWOS_GUNWUHA_DISPLAY_H
