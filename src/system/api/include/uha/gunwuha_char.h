//
//  gunwuha_char.h
//  Universal Hardware Abstraction for character device
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

#ifndef GUNWOS_GUNWUHA_CHAR_H
#define GUNWOS_GUNWUHA_CHAR_H

#include <stdgunw/types.h>

struct gnwDeviceUHA_char_in_desc {
    uint_32 _unused;
};

/*
    Character input device
*/
struct gnwDeviceUHA_char_in_routine {
    /*
        Determines if the device has data to be read
    
        Result:
            * 1 if data available, 0 otherwise
    */
    bool (*hasData)();

    /*
        Read character

        Params:
            * c - character buffer
             
        Result:
            * Number of characters read, 0 on error
    */
    size_t (*read)(const uint_8 * c);
};

struct gnwDeviceUHA_char_out_desc {
    uint_32 _unused;
};

/*
    Character output device
*/
struct gnwDeviceUHA_char_out_routine {
    /*
        Determines if the device is ready to be written to

        Result:
            * 1 if device ready, 0 otherwise
    */
    bool (*isReadyToWrite)();

    /*
        Write character to the device

        Params:
            * c - character to be written
    
        Result:
            * 1 if write succeeded, 0 otherwise
    */
    bool (*write)(const char c);
};

struct gnwDeviceUHA_char_in {
    struct gnwDeviceUHA_char_in_desc desc;
    struct gnwDeviceUHA_char_in_routine routine;
};

struct gnwDeviceUHA_char_out {
    struct gnwDeviceUHA_char_out_desc desc;
    struct gnwDeviceUHA_char_out_routine routine;
};

#endif // GUNWOS_GUNWUHA_CHAR_H