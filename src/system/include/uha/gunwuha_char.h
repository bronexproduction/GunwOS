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
#include <gunwstor.h>

/*
    Character input device
*/
struct gnwDeviceUHA_char_in {
    /*
        Determines if the device has data to be read
    
        Result:
            * 1 if data available, 0 otherwise
    */
    uint_8 (*hasData)();

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
struct gnwDeviceUHA_char_out {
    /*
        Determines if the device is ready to be written to

        Result:
            * 1 if device ready, 0 otherwise
    */
    uint_8 (*isReadyToWrite)();

    /*
        Write character to the device

        Params:
            * c - character to be written
    
        Result:
            * 1 if write succeeded, 0 otherwise
    */
    uint_8 (*write)(const uint_8 c);
};

#endif // GUNWOS_GUNWUHA_CHAR_H