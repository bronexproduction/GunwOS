//
//  gunwuha_drivectrl.h
//  Universal Hardware Abstraction for a storage device
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2021.
//

#ifndef GUNWOS_GUNWUHA_DRIVECTRL_H
#define GUNWOS_GUNWUHA_DRIVECTRL_H

#include <uha/gunwuha_drivectrl_desc.h>
#include <gunwstor.h>

struct gnwDeviceUHA_driveCtrl_routine {
    /*
        Check for drive presence

        Params:
            * index - number of drive in given controller
    
        Result:
            * 1 if drive present, 0 otherwise
    */
    bool (*drivePresent)(const uint_8 index);
    
    /*
        Check drive geometry

        Params:
            * index - number of drive in given controller
             
        Result:
            * drive geometry data
    */
    struct gnwStorGeometry (*driveGeometry)(const uint_8 index);

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

struct gnwDeviceUHA_driveCtrl {
    struct gnwDeviceUHA_driveCtrl_desc desc;
    struct gnwDeviceUHA_driveCtrl_routine routine;
};

#endif // GUNWOS_GUNWUHA_DRIVECTRL_H