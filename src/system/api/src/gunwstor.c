//
//  gunwstor.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#include <gunwstor.h>

struct gnwStorCHS lba2chs(const uint_32 lba, const uint_8 hpc, const uint_16 spt) {
    struct gnwStorCHS chs = {0};

    if (!hpc || !spt) {
        return chs;
    }

    uint_32 hsm = hpc * spt;
    chs.c = lba / hsm;
    chs.h = (lba % hsm) / spt;
    chs.s = (lba % hsm) % spt + 1;

    return chs;
}

size_t chs2lba(const uint_16 c, const uint_8 h, const uint_8 s, const uint_8 hpc, const uint_16 spt) {
    if (!hpc || !spt) {
        return 0;
    }
    
    if (!s) {
        return 0;
    }

    return ((c * hpc + h) * spt) + (s - 1);
}

size_t sectorAlignedBytes(const size_t bytes, const struct gnwStorGeometry geometry) {
    if (bytes % geometry.sectSizeBytes) {
        return bytes / geometry.sectSizeBytes + geometry.sectSizeBytes;
    }
    else {
        return bytes;
    }
}