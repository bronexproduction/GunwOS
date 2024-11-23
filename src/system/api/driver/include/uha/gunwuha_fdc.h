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

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_FDC GNW_UHA_EMPTY(FDC)
#define GNW_UHA_FDC_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_fdc_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_fdc {
    struct gnwDeviceUHA_fdc_desc desc;
    struct gnwDeviceUHA_fdc_routine routine;
};

#endif // GUNWOS_GUNWUHA_FDC_H