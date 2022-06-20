//
//  stormgr.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//

#include "stormgr.h"

#include "../../devmgr/devmgr.h"
#include "../../../driver/storage/floppy/floppy.h"
#include "../../../shared/log/log.h"
#include "../../../../../lib/stdgunw/mem.h"
#include "../ptable/ptable.h"
#include "../filsys/filsys.h"

#define MAX_VOLUMES 8
#define MAX_DRIVES 8
#define PTABLE_COUNT 0
#define FS_COUNT 0

static struct s_stor_drvDescriptor drives[MAX_DRIVES];
static struct s_stor_volDescriptor volumes[MAX_VOLUMES];
uint_8 driveCount;
uint_8 volumeCount;

static struct s_stor_ptable partitionTables[PTABLE_COUNT];
static struct s_stor_filsys fileSystems[FS_COUNT];

static uint_8 addDrive(const struct s_stor_drvDescriptor drive) {
    LOG_INFO("Storage manager: Adding drive")

    if (driveCount >= MAX_DRIVES) {
        return 0;
    }

    drives[driveCount++] = drive;

    return 1;
}

static uint_8 addVolume(const struct s_stor_volDescriptor volume) {
    LOG_INFO("Storage manager: Adding volume")

    if (volumeCount >= MAX_VOLUMES) {
        return 0;
    }

    volumes[volumeCount++] = volume;

    return 1;
}

struct s_stor_drvDescriptor s_stormgr_drive(const uint_32 driveID) {
    if (driveID >= MAX_DRIVES) {
        LOG_FATAL("Requested drive ID over limit")
        __builtin_unreachable();
    }

    return drives[driveID];
}

struct s_stor_volDescriptor s_stormgr_volume(const uint_32 volumeID) {
    if (volumeID >= MAX_VOLUMES) {
        LOG_FATAL("Requested volume ID over limit")
        __builtin_unreachable();
    }

    return volumes[volumeID];
}

void s_stormgr_scanDrives() {
    LOG_INFO("Storage manager: Scanning for storage drives...")
    
    for (int i = 0; i < s_devmgr_descriptorCount(); ++i) {
        const struct gnwDeviceDescriptor desc = s_devmgr_descriptorFor(i);

        if (desc.type == DEV_TYPE_FDC) {
            LOG_DEBUG("Storage manager: Found floppy disk controller")
            struct gnwDeviceUHA_fdc const fdcAPI = desc.api.fdc;
            if (!(fdcAPI.drivePresent) || 
                !(fdcAPI.driveGeometry)) {
                LOG_ERR("Storage manager: Floppy disk controller UHA unavailable");
                return;
            }
            for (uint_8 fddNo = 0; fddNo < 4; ++fddNo) {
                if (fdcAPI.drivePresent(fddNo)) {
                    struct s_stor_drvDescriptor driveDesc;
                    driveDesc.controllerDriverID = i;
                    driveDesc.controllerDriveIndex = fddNo;
                    driveDesc.device = VDE_FLOPPY;

                    struct gnwStorGeometry format = fdcAPI.driveGeometry(fddNo);
                    driveDesc.format = format;

                    struct s_stormgr_drvMeta meta;
                    meta.present = 1;
                    driveDesc.meta = meta;

                    if (!addDrive(driveDesc)) {
                        LOG_ERR("Storage manager: Unable to add drive due to unknown error")
                        return;
                    }
                }
            }
        }
    }

    LOG_INFO("Storage manager: Scanning for storage drives finished. ")
    #warning no formatted string available
    s_trm_putun(driveCount);
    LOG_INFO("drives present.")
}

void s_stormgr_scanVolumes() {
    LOG_INFO("Storage manager: Scanning for available volumes...")
    
    for (int i = 0; i < driveCount; ++i) {
        const struct s_stor_drvDescriptor driveDesc = drives[i];
        const struct gnwDeviceDescriptor devDesc = s_devmgr_descriptorFor(driveDesc.controllerDriverID);

        uint_8 flat = 1;
        for (uint_32 pt = 0; pt < PTABLE_COUNT; ++pt) {
            if (partitionTables[pt].detect(driveDesc)) {
                uint_32 count = partitionTables[pt].count(driveDesc);
                for (int_32 pIndex = 0; pIndex < count; ++pIndex) {
                    const struct s_stor_partitionDesc partDesc = partitionTables[pt].partition(driveDesc, pIndex);
                    
                    struct s_stor_volDescriptor volDesc;
                    
                    // TODO

                    if (!addVolume(volDesc)) {
                        LOG_ERR("Storage manager: Unable to add volume")
                        return;
                    }
                }

                flat = 0;
                break;
            }
        }

        if (flat) {
            for (uint_32 fs = 0; fs < FS_COUNT; ++fs) {
                if (fileSystems[fs].detect(driveDesc)) {
                    const struct s_stor_partitionDesc partDesc = fileSystems[fs].partition(driveDesc);
                    
                    struct s_stor_volDescriptor volDesc;
                    
                    // TODO

                    if (!addVolume(volDesc)) {
                        LOG_ERR("Storage manager: Unable to add volume")
                        return;
                    }
                }
            }
        }
    }

    LOG_INFO("Storage manager: Scanning for available volumes finished. ")
    #warning no formatted string available
    s_trm_putun(volumeCount);
    LOG_INFO("volumes present.")
}

void s_stormgr_init() {
    s_stormgr_scanDrives();
    s_stormgr_scanVolumes();
}

size_t s_stormgr_drvRead(const uint_32 driveID,
                         const size_t lba,
                         uint_8 *buffer,
                         const size_t byteCount,
                         struct gnwStorError *error) {

    struct s_stor_drvDescriptor drive = s_stormgr_drive(driveID);

    if (!drive.meta.present) {
        if (error) {
            error->code = DRIVE_NOT_PRESENT;
            error->internalCode = 0;
        }
        return 0;
    }

    struct gnwDeviceDescriptor driver = s_devmgr_descriptorFor(drive.controllerDriverID);

    if (!driver.api.storage.read) {
        if (error) {
            error->code = API_UNAVAILABLE;
            error->internalCode = 0;
        }
        return 0;
    }

    if (!drive.format.sectSize) {
        if (error) {
            error->code = INVALID_FORMAT;
            error->internalCode = 0;
        }
        return 0;
    }

    size_t singleReadSectorCount = 1;
    size_t sectorCount = ceil(byteCount/(double)drive.format.sectSize);
    size_t readInterationCount = ceil(sectorCount/(double)singleReadSectorCount);
    uint_8 buf[drive.format.sectSize * singleReadSectorCount];
    size_t bytesLeft = byteCount;
    size_t readBytes = 0;

    for (size_t i = 0; i < readInterationCount; ++i) {
        size_t currentBytes = driver.api.storage.read(driveID,
                                                      lba + (i * singleReadSectorCount),
                                                      singleReadSectorCount,
                                                      buf,
                                                      error);

        if ((currentBytes < singleReadSectorCount && currentBytes < bytesLeft) ||
            currentBytes > singleReadSectorCount) {
            return readBytes;
        }

        memcopy(buf, buffer + readBytes, currentBytes);

        bytesLeft -= currentBytes;
        readBytes += currentBytes;
    }

    return readBytes;
}

size_t s_stormgr_drvWrite(const uint_32 driveID,
                          const size_t lba,
                          const uint_8 * const buffer,
                          const size_t byteCount,
                          struct gnwStorError *error) {
    #warning not implemented yet
}
