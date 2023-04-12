//
//  storage.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include "storage.h"
#include <dev/dev.h>
#include <error/panic.h>
#include <mem.h>
#include "drive.h"
#include "volume.h"
#include "filesys.h"

static void addDrive(const size_t ctrlId, const size_t driveId) {
    size_t index = 0;
    for (; index < MAX_DRIVES; ++index) {
        if (!k_stor_drives[index].used) {
            break;
        }
    }
    if (index >= MAX_DRIVES) {
        OOPS("Drive count limit exceeded");
        return;
    }
    
    k_stor_drives[index].used = true;
    k_stor_drives[index].ctrlId = ctrlId;
    k_stor_drives[index].driveId = driveId;
}

static void detectDrives(const struct gnwDeviceUHADesc desc, bool (* const drivePresent)(uint_8)) {
    if (!drivePresent) {
        OOPS("Null-pointer parameter");
        return;
    }
    for (size_t driveIndex = 0; driveIndex < desc.storage.driveCount; ++driveIndex) {
        if (drivePresent(driveIndex)) {
            addDrive(desc.identifier, driveIndex);
        } 
    }
}

static void addVolume(const size_t driveId, const size_t fileSysId) {
    size_t index = 0;
    for (; index < MAX_VOLUMES; ++index) {
        if (!k_stor_volumes[index].used) {
            break;
        }
    }
    if (index >= MAX_VOLUMES) {
        OOPS("Volume count limit exceeded");
        return;
    }
    
    k_stor_volumes[index].used = true;
    k_stor_volumes[index].driveId = driveId;
    k_stor_volumes[index].fileSysId = fileSysId;
}

static void detectFileSystem(const struct gnwDeviceUHA_storCtrl * const uha, const uint_8 driveIndex) {
    if (!uha) {
        OOPS("Null-pointer parameter");
        return;
    }
    if (!uha->routine.driveGeometry ||
        !uha->routine.read) {
        OOPS("UHA invalid");
        return;
    }
    if (!uha->routine.driveGeometry) {
        OOPS("UHA invalid");
        return;
    }
    if (driveIndex >= MAX_DRIVES) {
        OOPS("Drive index over limit");
        return;
    }
    if (!k_stor_drives[driveIndex].used) {
        OOPS("Drive descriptor empty");
        return;
    }
    for (size_t fileSysIndex = 0; fileSysIndex < MAX_FILESYS; ++fileSysIndex) {
        if (!k_stor_fileSystems[fileSysIndex].used) {
            continue;
        }
        
        /*
            Load header bytes
        */
        
        if (!k_stor_fileSystems[fileSysIndex].desc.headerRange.length) {
            OOPS("Invalid header bytes value");
            return;
        }
        const struct gnwStorGeometry geometry = uha->routine.driveGeometry(k_stor_drives[driveIndex].driveId);
        size_t bytesToBeRead = sectorAlignedBytes(k_stor_fileSystems[fileSysIndex].desc.headerRange.offset + k_stor_fileSystems[fileSysIndex].desc.headerRange.length,
                                                  geometry);
        uint_8 readBuffer[bytesToBeRead];
        uint_8 headerBuffer[k_stor_fileSystems[fileSysIndex].desc.headerRange.length];

        struct gnwStorError err;
        #warning If header bytes outside the first sector then LBA can be optimized
        const size_t bytesRead = uha->routine.read(k_stor_drives[driveIndex].driveId, 
                                                   0,
                                                   bytesToBeRead / geometry.sectSizeBytes, 
                                                   readBuffer, 
                                                   &err);
        if (err.code != GSEC_NONE) {
            if (uha->desc.removableMedia) {
                /*
                    Possibly no media inserted
                */
                #warning this error should use GSEC_MEDIA_NOT_PRESENT
                return;
            } else {
                OOPS("Header read error");
                return;
            }
        } else if (bytesRead != bytesToBeRead) {
            OOPS("Header read error");
            return;
        }

        memcopy(readBuffer + k_stor_fileSystems[fileSysIndex].desc.headerRange.offset, 
                headerBuffer, 
                k_stor_fileSystems[fileSysIndex].desc.headerRange.length);

        if (k_stor_fileSystems[fileSysIndex].desc.detect(headerBuffer, &geometry)) {
            addVolume(driveIndex, fileSysIndex);
            break;
        }
    }
}

static void detectVolumes(const struct gnwDeviceUHA_storCtrl * const uha) {
    
    /*
        Detect volumes (filesystems)

        NOTE: Restriction to single-volume drives
    */
    
    for (uint_8 driveIndex = 0; driveIndex < MAX_DRIVES; ++driveIndex) {
        if (k_stor_drives[driveIndex].used) {
            detectFileSystem(uha, driveIndex);
        }
    }
}

void k_stor_init() {
    
    /*
        Detect available drives
    */
    
    struct gnwDeviceUHADesc desc;
    enum gnwDeviceError err = k_dev_getByType(DEV_TYPE_STORAGE, &desc);
    if (err != GDE_NONE) {
        OOPS("Error getting storage devices");
        return;
    }

    struct gnwDeviceUHA uha;
    err = k_dev_getUHAForId(desc.identifier, &uha);
    if (err != GDE_NONE) {
        OOPS("Error getting storage device API");
        return;
    }

    detectDrives(desc, uha.storCtrl.routine.drivePresent);
    detectVolumes(&uha.storCtrl);
}
