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

enum k_stor_error k_stor_volume_readHeader(const size_t volumeId, uint_8 * const header) {
    if (!header) {
        OOPS("Nullptr");
        return SE_UNKNOWN;
    }
    if (!k_stor_volume_validateId(volumeId)) {
        OOPS("Invalid volume");
        return SE_UNKNOWN;
    }

    // struct gnwFileSystemDescriptor fsDesc = fileSys[fileSysId].desc;
        
    // struct gnwDeviceUHA_driveCtrl uha; {
    //     const struct gnwDeviceError err = k_dev_getUHAForId(desc.identifier, &uha);
    // }
    

    // struct gnwStorError err;
    // #warning If header bytes outside the first sector then LBA can be optimized
    // const size_t bytesRead = uha->routine.read(drives[driveIndex].driveId, 
    //                                                0,
    //                                                bytesToBeRead / geometry.sectSizeBytes, 
    //                                                readBuffer, 
    //                                                &err);
    //     if (err.code != GSEC_NONE) {
    //         if (uha->desc.removableMedia) {
    //             /*
    //                 Possibly no media inserted
    //             */
    //             #warning this error should use GSEC_MEDIA_NOT_PRESENT
    //             return;
    //         } else {
    //             OOPS("Header read error");
    //             return;
    //         }
    //     } else if (bytesRead != bytesToBeRead) {
    //         OOPS("Header read error");
    //         return;
    //     }

    //     memcopy(readBuffer + fileSys[fileSysIndex].desc.headerRange.offset, 
    //             headerBuffer, 
    //             fileSys[fileSysIndex].desc.headerRange.length);

                return SE_NONE;

}
