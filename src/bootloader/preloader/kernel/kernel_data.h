//
//  kernel_data.h
//  GunwOS Bootloader
//  Part of GunwOS project
//
//  Created by Artur Danielewski on 03.01.2021.
//

#include "types.h"

#define E820_MAX_ENTRIES    128

enum k_krn_emsMapRangeType {
    EMRT_NONE = 0,
    EMRT_MEMORY = 1,
    EMRT_RESERVED = 2
};
_Static_assert(sizeof(enum k_krn_emsMapRangeType) == sizeof(int_32), "Unexpected enum k_krn_emsMapRangeType size");

struct __attribute__((packed)) k_krn_emsMapEntry {
    uint_32 baseAddrLow;
    uint_32 baseAddrHigh;
    uint_32 lengthLow;
    uint_32 lengthHigh;
    enum k_krn_emsMapRangeType type;
    uint_32 _unused;
};

struct __attribute__((packed)) k_krn_bootData {
    uint_16 convMemAvailable;
    struct k_krn_emsMapEntry emsMap[E820_MAX_ENTRIES];
};
