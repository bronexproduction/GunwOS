//
//  gunwuha_display.h
//  Universal Hardware Abstraction for a display
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.
//

#ifndef GUNWOS_GUNWUHA_DISPLAY_H
#define GUNWOS_GUNWUHA_DISPLAY_H

#include <stdgunw/types.h>

enum gnwDeviceUHA_display_type {
    CHARACTER,
    GRAPHICS
};

struct gnwDeviceUHA_display {
    /*
        Display resolution
    
        Result:
            * X and Y display dimensions
    */
    point_t dimensions;

    /*
        Display type
    
        Result:
            * Display type - enum gnwDeviceUHA_display_type
    */
    enum gnwDeviceUHA_display_type type;

    /*
        Format
    
        Result:
            * Pixel/character format size (bytes)
    */
    size_t formatBytes;

    /*
        Update the framebuffer
    
        Params:
            * Framebuffer data (X*Y*formatBytes)
    */
    size_t (*update)(const uint_8 * const buffer);
};

#endif // GUNWOS_GUNWUHA_DISPLAY_H