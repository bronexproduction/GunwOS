//
//  volume.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "volume.h"
#include "drive.h"
#include "filesys.h"
#include <error/panic.h>
#include <dev/dev.h>
#include <mem.h>

struct k_stor_volume k_stor_volumes[MAX_VOLUMES];

bool k_stor_volume_validateId(const size_t volumeId) {
    if (volumeId >= MAX_VOLUMES) {
        return false;
    }
    if (!k_stor_volumes[volumeId].used) {
        return false;
    }
    if (!k_stor_drive_validateId(k_stor_volumes[volumeId].driveId)) {
        return false;
    }
    if (!k_stor_fileSys_validateId(k_stor_volumes[volumeId].fileSysId)) {
        return false;
    }

    return true;
}

/*
    Read bytes from volume

    NOTE: Assumes all the identifiers and buffers validated prior to the call
*/
static enum k_stor_error readBytes(const size_t volumeId, const range_size_t range, uint_8 * const buffer) {
    const struct k_stor_volume volume = k_stor_volumes[volumeId];
    const struct k_stor_drive drive = k_stor_drives[volume.driveId];
    struct gnwDeviceUHA uha; {
        const enum gnwDeviceError err = k_dev_getUHAForId(drive.ctrlId, &uha);
        if (err != GDE_NONE) {
            return SE_INVALID_DEVICE;
        }
    }

    const struct gnwStorGeometry geometry = uha.storage.routine.driveGeometry(drive.driveId);
    
    const size_t offsetLBA = range.offset / geometry.sectSizeBytes;
    range_size_t adjustedRange;
    adjustedRange.offset = range.offset % geometry.sectSizeBytes;
    adjustedRange.length = range.length;
    
    const size_t bytesToRead = sectorAlignedBytes(adjustedRange.offset + adjustedRange.length, geometry);
    uint_8 readBuffer[bytesToRead]; {
        struct gnwStorError err;
        const size_t bytesRead = uha.storage.routine.read(drive.driveId,
                                                          offsetLBA,
                                                          bytesToRead / geometry.sectSizeBytes, 
                                                          readBuffer, 
                                                          &err);

        if (err.code != GSEC_NONE) {
            #warning TO BE IMPLEMENTED
            // if (uha->desc.removableMedia) {
            //     /*
            //         Possibly no media inserted
            //     */
            //     #warning this error should use GSEC_MEDIA_NOT_PRESENT
            //     return;
            // } else {
            //     OOPS("Header read error");
            //     return;
            // }
        } else if (bytesRead != bytesToRead) {
            // OOPS("Header read error");
            // return;
        }
    }
    
    memcopy(readBuffer + adjustedRange.offset,
            buffer, 
            adjustedRange.length);

    return SE_NONE;
}

enum k_stor_error k_stor_volume_readHeader(const size_t volumeId, uint_8 * const headerBuffer) {
    if (!headerBuffer) {
        OOPS("Nullptr");
        return SE_UNKNOWN;
    }
    if (!k_stor_volume_validateId(volumeId)) {
        OOPS("Invalid volume");
        return SE_UNKNOWN;
    }


    const struct k_stor_fileSystem fileSystem = k_stor_fileSystems[k_stor_volumes[volumeId].fileSysId];
    return readBytes(volumeId, fileSystem.desc.headerRange, headerBuffer);
}

enum k_stor_error k_stor_volume_readBytes(const size_t volumeId, const range_size_t range, uint_8 * const buffer) {
    if (!buffer) {
        OOPS("Nullptr");
        return SE_UNKNOWN;
    }
    if (!k_stor_volume_validateId(volumeId)) {
        OOPS("Invalid volume");
        return SE_UNKNOWN;
    }

    return readBytes(volumeId, range, buffer);
}

enum k_stor_error k_stor_volume_readSector(const size_t volumeId, const size_t sector, uint_8 * const buffer) {
    const struct k_stor_volume volume = k_stor_volumes[volumeId];
    const struct k_stor_drive drive = k_stor_drives[volume.driveId];
    struct gnwDeviceUHA uha; {
        const enum gnwDeviceError err = k_dev_getUHAForId(drive.ctrlId, &uha);
        if (err != GDE_NONE) {
            return SE_INVALID_DEVICE;
        }
    }

    const struct gnwStorGeometry geometry = uha.storage.routine.driveGeometry(drive.driveId);
    range_size_t readRange;

    readRange.offset = sector * geometry.sectSizeBytes;
    readRange.length = geometry.sectSizeBytes;

    return k_stor_volume_readBytes(volumeId, readRange, buffer);
}