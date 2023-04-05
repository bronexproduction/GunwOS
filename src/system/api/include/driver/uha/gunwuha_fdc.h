//
//  gunwuha_fdc.h
//  Universal Hardware Abstraction for floppy drive controller
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_FDC_H
#define GUNWOS_GUNWUHA_FDC_H

#include <uha/gunwuha_fdc_desc.h>
#include <gunwstor.h>

struct gnwDeviceUHA_fdc_routine {
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
};

struct gnwDeviceUHA_fdc {
    struct gnwDeviceUHA_fdc_desc desc;
    struct gnwDeviceUHA_fdc_routine routine;
};

#endif // GUNWOS_GUNWUHA_FDC_H