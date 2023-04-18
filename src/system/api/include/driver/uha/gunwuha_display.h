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
        Loads list of supported formats under given array pointer
    
        Note: result array should be of size equal (desc.supportedFormatCount * sizeof(enum gnwDeviceUHA_display_format))
    */
    void (*supportedFormats)(enum gnwDeviceUHA_display_format * const result);

    /*
        Display resolution for given format (if supported)
    
        Result:
            * X and Y display dimensions, if format supported
            * { -1, -1 } otherwise
    */
    point_t (*dimensionsForFormat)(const gnwDeviceUHA_display_format);

    /*
        Sets the desired display format

        Result:
            * true on success, false otherwise
    */
    bool (*setFormat)(enum gnwDeviceUHA_display_format);
};

struct gnwDeviceUHA_display {
    struct gnwDeviceUHA_display_desc desc;
    struct gnwDeviceUHA_display_routine routine;
};

#endif // GUNWOS_GUNWUHA_DISPLAY_H
