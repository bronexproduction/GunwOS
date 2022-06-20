//
//  storage.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//

#include "storage.h"
#include "../../../include/gunwstor.h"

#include "../../shared/log/log.h"

struct gnwStorCHS k_utl_lba2chs(const uint_32 lba, const uint_8 hpc, const uint_16 spt) {
    if (!hpc || !spt) {
        LOG_FATAL("Unexpected drive parameters")
        __builtin_unreachable();
    }

    struct gnwStorCHS chs;

    uint_32 hsm = hpc * spt;
    chs.c = lba / hsm;
    chs.h = (lba % hsm) / spt;
    chs.s = (lba % hsm) % spt + 1;

    return chs;
}

uint_32 k_utl_chs2lba(const uint_16 c, const uint_8 h, const uint_8 s, const uint_8 hpc, const uint_16 spt) {
    if (!hpc || !spt) {
        LOG_FATAL("Unexpected drive parameters")
        __builtin_unreachable();
    }
    
    if (!s) {
        LOG_FATAL("Illegal CHS sector value")
        __builtin_unreachable();
    }

    return ((c * hpc + h) * spt) + (s - 1);
}
