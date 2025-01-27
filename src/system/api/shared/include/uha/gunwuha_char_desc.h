//
//  gunwuha_char_desc.h
//  Universal Hardware Abstraction for character device
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_CHAR_DESC_H
#define GUNWOS_GUNWUHA_CHAR_DESC_H

#include <types.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_CHAR_IN_DESC_EMPTY { 0 }
#define GNW_UHA_CHAR_OUT_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_charIn_desc {
    uint_32 _unused;
};

struct gnwDeviceUHA_charOut_desc {
    uint_32 _unused;
};

#endif // GUNWOS_GUNWUHA_CHAR_DESC_H