//
//  storage.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//

#ifndef DEV_DRV_STOR_STORAGE_H
#define DEV_DRV_STOR_STORAGE_H

#include "../../../../lib/stdgunw/types.h"

/*
    Converts LBA (Logical Block Addressing)
    to CHS (Cylinder - Head - Sector)

    Params:
    * lba - LBA address to be converted
    * hpc - Head-per-cylinder count
    * spt - Sector-per-track count
*/
struct gnwStorCHS k_utl_lba2chs(const uint_32 lba, const uint_8 hpc, const uint_16 spt);

/*
    Converts CHS (Cylinder - Head - Sector)
    to LBA (Logical Block Addressing)

    Params:
    * c - Cylinder value
    * h - Head value
    * s - Sector value
    * hpc - Head-per-cylinder count
    * spt - Sector-per-track count
*/
uint_32 k_utl_chs2lba(const uint_16 c, const uint_8 h, const uint_8 s, const uint_8 hpc, const uint_16 spt);

#endif // DEV_DRV_STOR_STORAGE_H
