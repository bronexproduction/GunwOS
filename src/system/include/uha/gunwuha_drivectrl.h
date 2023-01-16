//
//  gunwuha_drivectrl.h
//  Universal Hardware Abstraction for a storage device
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2021.
//

#ifndef GUNWOS_GUNWUHA_DRIVECTRL_H
#define GUNWOS_GUNWUHA_DRIVECTRL_H

#include <stdgunw/types.h>
#include "../gunwstor.h"

struct gnwDeviceUHA_driveCtrl {
    /*
        Read data from drive

        Params:
            * index - number of drive in given controller
            * lba - sector address
            * count - number of sectors to be read
            * buffer - data output location
            * error (out parameter) - read error
    
        Result:
            * number of sectors read
    */
    size_t (*read)(const uint_8 index, 
                   const size_t lba, 
                   const size_t count, 
                   uint_8 * const buffer, 
                   struct gnwStorError *error);
};

#endif // GUNWOS_GUNWUHA_DRIVECTRL_H