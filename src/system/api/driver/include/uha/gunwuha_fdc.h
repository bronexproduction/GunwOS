//
//  gunwuha_fdc.h
//  Universal Hardware Abstraction for floppy drive controller
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_FDC_H
#define GUNWOS_GUNWUHA_FDC_H

#include "../../../app/include/uha/gunwuha_fdc_desc.h"
#include "../../../app/include/gunwstor.h"

struct gnwDeviceUHA_fdc_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_fdc {
    struct gnwDeviceUHA_fdc_desc desc;
    struct gnwDeviceUHA_fdc_routine routine;
};

#endif // GUNWOS_GUNWUHA_FDC_H