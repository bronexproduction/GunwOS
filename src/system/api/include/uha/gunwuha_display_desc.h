//
//  gunwuha_display_desc.h
//  Universal Hardware Abstraction for a display
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_DISPLAY_DESC_H
#define GUNWOS_GUNWUHA_DISPLAY_DESC_H

#include <types.h>

/*
    Display format

    Bit mask:
    | 7            | 6-0                                 |
    | Mode         | Resolution and color format variant |
    | 0 - text     |                                     |
    | 1 - graphics |                                     |
*/

#define _FMT_TEXT(VAL) (VAL)
#define _FMT_GRAP(VAL) ((1 << 7) & VAL)

#define GDD_FMT_ISGRAP(VAL) (((1 << 7) & VAL) != 0)
#define GDD_FMT_ISTEXT(VAL) (!GDD_FMT_ISGRAP(VAL))

/*
    Display format

    Naming scheme:
    {1}_H{2}V{3}{4}{5}B{6}
    1 - Mode: [TEXT,GRAPHICS]
    2 - Horizontal resolution (number of characters/points)
    3 - Vertical resolution (number of characters/points)
    4 - Color type: [(C)olor,(G)reyscale]
    5 - Number of colors/values on a greyscale
*/
enum gnwDeviceUHA_display_format {
    TEXT_H80V25C16          = _FMT_TEXT(1),
    GRAPHICS_H320V200C16    = _FMT_GRAP(1)
};

/*
    Structure for text mode character storage

    Note: Values are expected to be converted
          to match the expected memory layout
          for given device
*/
struct gnwDeviceUHA_display_character {
    uint_8 bgColor;
    uint_8 charColor;
    uint_8 character;
};

/*
    Structure for graphics mode pixel storage

    Note: Values are expected to be converted
          to match the expected memory layout
          for given device
*/
struct gnwDeviceUHA_display_pixel {
    uint_32 color;
};

struct gnwDeviceUHA_display_desc {
    /*
        Display resolution
    
        Result:
            * X and Y display dimensions
    */
    point_t dimensions;

    /*
        Display format
    
        Result:
            * Display format - enum gnwDeviceUHA_display_format
    */
    enum gnwDeviceUHA_display_format format;
};

#endif // GUNWOS_GUNWUHA_DISPLAY_DESC_H
