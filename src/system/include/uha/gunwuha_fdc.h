//
//  gunwuha_fdc.h
//  Universal Hardware Abstraction for floppy drive controller
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_FDC_H
#define GUNWOS_GUNWUHA_FDC_H

#include <stdgunw/types.h>
#include "../gunwstor.h"

struct gnwDeviceUHA_fdc {
    /*
        Check for drive presence

        Params:
            * index - number of drive in given controller
    
        Result:
            * 1 if drive present, 0 otherwise
    */
    uint_8 (*drivePresent)(const uint_8 index);

    /*
        Check drive geometry

        Params:
            * index - number of drive in given controller
             
        Result:
            * drive geometry data
    */
    struct gnwStorGeometry (*driveGeometry)(const uint_8 index);
};

#endif // GUNWOS_GUNWUHA_FDC_H