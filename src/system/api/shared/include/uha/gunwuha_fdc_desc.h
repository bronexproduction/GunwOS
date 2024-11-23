//
//  gunwuha_fdc_desc.h
//  Universal Hardware Abstraction for floppy drive controller
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_FDC_DESC_H
#define GUNWOS_GUNWUHA_FDC_DESC_H

#include <types.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_FDC_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_fdc_desc {
    uint_32 _unused;
};

#endif // GUNWOS_GUNWUHA_FDC_DESC_H
