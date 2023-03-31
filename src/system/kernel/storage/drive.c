//
//  drive.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "drive.h"
#include <dev/dev.h>

struct k_stor_drive k_stor_drives[MAX_DRIVES];

bool k_stor_drive_validateId(const size_t driveId) {
    if (driveId >= MAX_DRIVES) {
        return false;
    }
    if (!k_stor_drives[driveId].used) {
        return false;
    }

    struct gnwDeviceUHADesc desc;
    enum gnwDeviceError err = k_dev_getById(k_stor_drives[driveId].ctrlId, &desc);
    if (err != GDE_NONE) {
        return false;
    }
    if (!(desc.type & DEV_TYPE_STORAGE)) {
        return false;
    }
    if (k_stor_drives[driveId].driveId >= desc.storage.driveCount) {
        return false;
    }

    return true;
}
