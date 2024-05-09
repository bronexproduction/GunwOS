//
//  kernel_data.h
//  GunwOS Bootloader
//  Part of GunwOS project
//
//  Created by Artur Danielewski on 03.01.2021.
//

#include "types.h"

#define E820_MAX_ENTRIES    128

struct __attribute__((packed)) k_krn_emsMapEntry {
    uint_64 something1;
    uint_64 something2;
    uint_32 something3;
    uint_32 something4;
};

struct __attribute__((packed)) k_krn_bootData {
    uint_16 convMemAvailable;
    struct k_krn_emsMapEntry emsMap[E820_MAX_ENTRIES];
};
