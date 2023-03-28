//
//  storage.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include "storage.h"
#include <dev/dev.h>
#include <error/panic.h>

#define MAX_DRIVES 8
#define MAX_VOLUMES 16
#define MAX_FILESYS 2

static struct drive {
    /*
        Entry status
    */
    bool used;

    /*
        Drive controller device identifier
    */
    size_t ctrlId;

    /*
        Drive identifier in controller
    */
    size_t driveId;
} drives[MAX_DRIVES];

static struct volume {
    /*
        Entry status
    */
    bool used;

    /*
        Drive identifier
    */
    size_t driveId;

    /*
        Volume identifier/index in controller
    */
    size_t volumeId;
} volumes[MAX_VOLUMES];

static struct filesystem {
    /*
        Entry status
    */
    bool used;
} fileSys[MAX_VOLUMES];

enum k_stor_error k_stor_fileSysInstall(const struct gnwFileSystemDescriptor * const desc) {
    (void)desc;
    (void)fileSys;
    return SE_NONE;
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

    for (size_t driveIndex = 0; driveIndex < desc.storage.driveCount; ++driveIndex) {
        if (uha.storage.routine.drivePresent(driveIndex)) {
            /*
                Add drive
            */
            size_t volumeIndex = 0;
            for (; volumeIndex < MAX_DRIVES; ++volumeIndex) {
                if (!drives[volumeIndex].used) {
                    break;
                }
            }
            if (volumeIndex >= MAX_DRIVES) {
                OOPS("Volume count limit exceeded");
                return;
            }
            drives[volumeIndex].used = true;
            drives[volumeIndex].ctrlId = desc.identifier;
            drives[volumeIndex].driveId = driveIndex;
        } 
    }

    /*
        Detect filesystems

        NOTE: Restriction to single-volume drives
    */

    (void)volumes;
}
