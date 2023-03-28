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
    
    /*
        File system identifier
    */
    size_t fileSysId;
} volumes[MAX_VOLUMES];

static struct filesystem {
    /*
        Entry status
    */
    bool used;

    /*
        File system descriptor
    */
    struct gnwFileSystemDescriptor desc;
} fileSys[MAX_FILESYS];

enum k_stor_error k_stor_fileSysInstall(const struct gnwFileSystemDescriptor * const desc) {
    size_t fileSysIndex = 0;
    for (; fileSysIndex < MAX_FILESYS; ++fileSysIndex) {
        if (!fileSys[fileSysIndex].used) {
            break;
        }
    }
    if (fileSysIndex >= MAX_FILESYS) {
        OOPS("File system count limit exceeded");
        return SE_LIMIT_REACHED;
    }
            
    fileSys[fileSysIndex].used = true;
    fileSys[fileSysIndex].desc = *desc;
    return SE_NONE;
}

static void addDrive(const size_t ctrlId, const size_t driveId) {
    size_t index = 0;
    for (; index < MAX_DRIVES; ++index) {
        if (!drives[index].used) {
            break;
        }
    }
    if (index >= MAX_DRIVES) {
        OOPS("Volume count limit exceeded");
        return;
    }
    
    drives[index].used = true;
    drives[index].ctrlId = ctrlId;
    drives[index].driveId = driveId;
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

static void detectFileSystem(const struct gnwDeviceUHA_driveCtrl * const uha, const size_t driveIndex) {
    if (driveIndex >= MAX_DRIVES) {
        OOPS("Drive index over limit");
        return;
    }
    if (!drives[driveIndex].used) {
        OOPS("Drive descriptor empty");
        return;
    }
    for (size_t fileSysIndex = 0; fileSysIndex < MAX_FILESYS; ++fileSysIndex) {
        if (!fileSys[fileSysIndex].used) {
            continue;
        }
        /*
            Load header bytes
        */
        
        uint_8 headerBytes[51+0x0B] = {0};
        if (fileSys[fileSysIndex].desc.detect(headerBytes)) {
            
            /*
                Add volume
            */
            (void)volumes;
            break;
        }
    }
}

static void detectVolumes(const struct gnwDeviceUHA_driveCtrl * const uha) {
    
    /*
        Detect volumes (filesystems)

        NOTE: Restriction to single-volume drives
    */
    
    for (size_t driveIndex = 0; driveIndex < MAX_DRIVES; ++driveIndex) {
        if (drives[driveIndex].used) {
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

    detectDrives(desc, uha.storage.routine.drivePresent);
    detectVolumes(&uha.storage);
}
