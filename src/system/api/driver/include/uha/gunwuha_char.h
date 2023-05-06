//
//  gunwuha_char.h
//  Universal Hardware Abstraction for character device
//  GunwOS
//
//  Created by Artur Danielewski on 13.01.2023.
//

#ifndef GUNWOS_GUNWUHA_CHAR_H
#define GUNWOS_GUNWUHA_CHAR_H

#include <uha/gunwuha_char_desc.h>

/*
    Character input device
*/
struct gnwDeviceUHA_charIn_routine {
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

/*
    Character output device
*/
struct gnwDeviceUHA_charOut_routine {
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

struct gnwDeviceUHA_charIn {
    struct gnwDeviceUHA_charIn_desc desc;
    struct gnwDeviceUHA_charIn_routine routine;
};

struct gnwDeviceUHA_charOut {
    struct gnwDeviceUHA_charOut_desc desc;
    struct gnwDeviceUHA_charOut_routine routine;
};

#endif // GUNWOS_GUNWUHA_CHAR_H