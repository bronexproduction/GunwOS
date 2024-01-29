//
//  volume.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef VOLUME_H
#define VOLUME_H

#include <storage/storage.h>

#define MAX_VOLUMES 16

extern struct k_stor_volume {
    /*
        Entry status
    */
    bool used;

    /*
        Drive identifier
    */
    size_t driveId;
    
    /*
        File system identifier
    */
    size_t fileSysId;
} k_stor_volumes[MAX_VOLUMES];

enum k_stor_error k_stor_volume_readHeader(const size_t volumeId, uint_8 * const header);
enum k_stor_error k_stor_volume_readBytes(const size_t volumeId, const range_size_t range, uint_8 * const buffer);
enum k_stor_error k_stor_volume_readSector(const size_t volumeId, const size_t lba, const size_t count, uint_8 * const buffer);

bool k_stor_volume_validateId(const size_t volumeId);

#endif // VOLUME_H
