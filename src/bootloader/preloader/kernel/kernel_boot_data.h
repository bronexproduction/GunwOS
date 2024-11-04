//
//  kernel_boot_data.h
//  GunwOS Bootloader
//  Part of GunwOS project
//
//  Created by Artur Danielewski on 03.01.2021.
//

#ifndef KERNEL_BOOT_DATA_H
#define KERNEL_BOOT_DATA_H

#include "types.h"

#define MMAP_MAX_ENTRIES    128

enum k_krn_memMapRangeType {
    MMRT_NONE = 0,
    MMRT_MEMORY = 1,
    MMRT_RESERVED = 2
};
_Static_assert(sizeof(enum k_krn_memMapRangeType) == sizeof(int_32), "Unexpected enum k_krn_memMapRangeType size");

struct __attribute__((packed)) k_krn_memMapEntry {
    uint_32 baseAddrLow;
    uint_32 baseAddrHigh;
    uint_32 lengthLow;
    uint_32 lengthHigh;
    enum k_krn_memMapRangeType type;
    uint_32 _unused;
};

struct __attribute__((packed)) k_krn_bootData {
    uint_16 convMemAvailable;
    struct k_krn_memMapEntry memMap[MMAP_MAX_ENTRIES];
};

#endif // KERNEL_BOOT_DATA_H
