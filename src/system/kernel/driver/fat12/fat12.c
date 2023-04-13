//
//  fat12.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include <driver/gunwfilesys.h>
#include <string.h>
#include <utils.h>
#include <mem.h>
#include <gunwstor.h>
#include "model.h"

#define FILE_SYSTEM_NAME "FAT12   "
#define FILE_SYSTEM_NAME_BYTES 8
#define FILE_NAME_MAX_LENGTH 8
#define FILE_EXTENSION_MAX_LENGTH 3
#define MIN_FILE_CLUSTER 2
#define EXTENDED_BOOT_SIGNATURE 0x29
#define BYTES_TO_FAT_ENTRIES(BYTES) (BYTES * 2 / 3)
#define BITS_PER_FAT_ENTRY 12

static range_size_t fatRange(const uint_8 * const headerBytes) {
    range_size_t range;

    if (!headerBytes) {
        range.offset = 0;
        range.length = 0;
        return range;
    }

    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    range.offset = bpb->reservedLogicalSectors * bpb->bytesPerLogicalSector;
    range.length = bpb->numberOfFATs * bpb->logicalSectorsPerFAT * bpb->bytesPerLogicalSector;

    return range;
}

static range_size_t directoryRange(const uint_8 * const headerBytes) {
    range_size_t range;

    if (!headerBytes) {
        range.offset = 0;
        range.length = 0;
        return range;
    }

    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    const range_size_t fatTablesRange = fatRange(headerBytes);

    range.offset = fatTablesRange.offset + fatTablesRange.length;
    range.length = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);

    return range;
}

static size_t clusterStartSector(const uint_8 * const headerBytes,
                                 const size_t cluster) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    const range_size_t dirRange = directoryRange(headerBytes);
    const size_t firstClusterSector = (dirRange.offset + dirRange.length) / bpb->bytesPerLogicalSector;

    return firstClusterSector + (cluster - MIN_FILE_CLUSTER) * bpb->logicalSectorsPerCluster;
}

static size_t nextCluster(const uint_8 * const fatBytes,
                          const size_t cluster) {
    uint_16 lsbIndex = (3 * cluster) / 2;
    uint_8 lsb = fatBytes[lsbIndex];
    uint_8 msb = fatBytes[lsbIndex + 1]; 

    return (cluster % 2) ? (msb << 4) | (lsb >> 4) : ((msb & 0xF) << 8) | lsb;
}

static bool fatVerify(const uint_8 * const headerBytes,
                      const uint_8 * const fatBytes) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    
    for (size_t fatIndex = 1; fatIndex < bpb->numberOfFATs; ++fatIndex) {
        return !strcmpl((char *)fatBytes, 
                        (char *)fatBytes + fatIndex * bpb->logicalSectorsPerFAT * bpb->bytesPerLogicalSector, 
                        bpb->logicalSectorsPerFAT * bpb->bytesPerLogicalSector);
    }
    
    return false;
}

static struct gnwFileSystemLocation fileStartLocation(const uint_8 * const headerBytes,
                                                      const struct gnwFileInfo * const fileInfo) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    struct gnwFileSystemLocation startLocation;

    startLocation.allocUnit = fileInfo->allocUnit;
    startLocation.sector = clusterStartSector(headerBytes, startLocation.allocUnit);
    startLocation.sizeBytes = bpb->bytesPerLogicalSector;

    return startLocation;
}
    
static struct gnwFileSystemLocation nextLocation(const uint_8 * const headerBytes,
                                                 const uint_8 * const fatBytes,
                                                 const struct gnwFileSystemLocation currentLocation) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    struct gnwFileSystemLocation nextLocation;

    if ((currentLocation.sector + 1) % bpb->logicalSectorsPerCluster) {
        nextLocation.allocUnit = currentLocation.allocUnit;
        nextLocation.sector = currentLocation.sector + 1;
    }
    else {
        nextLocation.allocUnit = nextCluster(fatBytes, currentLocation.allocUnit);
        nextLocation.sector = clusterStartSector(headerBytes, nextLocation.allocUnit);
    }
    nextLocation.sizeBytes = bpb->bytesPerLogicalSector;

    return nextLocation;
}

static bool isValidForRead(const uint_8 * const headerBytes,
                           const struct gnwFileSystemLocation location) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    const range_size_t dirRange = directoryRange(headerBytes);
    const size_t dataSectors = bpb->totalLogicalSectors - (dirRange.offset + dirRange.length) / bpb->bytesPerLogicalSector;
    const size_t lastCluster = dataSectors / bpb->logicalSectorsPerCluster + 1;

    return IN_RANGE(MIN_FILE_CLUSTER, location.allocUnit, MIN(lastCluster, 0xFEF));
}

static bool isEOF(const struct gnwFileSystemLocation location) {
    return IN_RANGE(0xFF8, location.allocUnit, 0xFFF);
}

static bool validateDirEntry(const struct dos_4_0_ebpb_t * const bpb,
                             const struct fat12_dir_t * const entry) {
    if (entry->firstLogicalCluster < MIN_FILE_CLUSTER) {
        return false;
    }
    const size_t fatSectors = bpb->numberOfFATs * bpb->logicalSectorsPerFAT;
    const size_t dirBytes = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);
    const size_t dirSectors = dirBytes / bpb->bytesPerLogicalSector;
    const size_t maxFatEntry = bpb->totalLogicalSectors - bpb->reservedLogicalSectors - fatSectors - dirSectors + 1;
    if (entry->firstLogicalCluster > maxFatEntry) {
        return false;
    }
    if (!entry->fileSizeBytes) {
        return false;
    }
    
    return true;
}

static enum gnwFileErrorCode fileInfo(const uint_8 * const headerBytes,
                                      const uint_8 * const directoryBytes,
                                      const char * const name,
                                      const char * const extension,
                                      struct gnwFileInfo * const fileInfo) {
    if (!headerBytes ||
        !directoryBytes ||
        !name ||
        !extension ||
        !fileInfo) {
        return GFEC_INVALID_PARAMETER;
    }

    char fatName[FILE_NAME_MAX_LENGTH];
    char fatExtension[FILE_EXTENSION_MAX_LENGTH];
    memcopy(name, fatName, FILE_NAME_MAX_LENGTH);
    memcopy(extension, fatExtension, FILE_EXTENSION_MAX_LENGTH);
    strreplacel(fatName, FILE_NAME_MAX_LENGTH, 0, ' ');
    strreplacel(fatExtension, FILE_EXTENSION_MAX_LENGTH, 0, ' ');

    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    for (size_t dirIndex = 0; dirIndex < bpb->maxRootDirectoryEntries; ++dirIndex) {
        const struct fat12_dir_t * const dirEntry = (struct fat12_dir_t *)(directoryBytes + dirIndex * sizeof(struct fat12_dir_t));
        if (!strcmpl(fatName, (char *)dirEntry->filename, FILE_NAME_MAX_LENGTH) &&
            !strcmpl(fatExtension, (char *)dirEntry->extension, FILE_EXTENSION_MAX_LENGTH)) {
            if (!validateDirEntry(bpb, dirEntry)) {
                return GFEC_NOT_FOUND;
            }

            fileInfo->sizeBytes = dirEntry->fileSizeBytes;
            fileInfo->allocUnit = dirEntry->firstLogicalCluster;

            return GFEC_NONE;
        }
    }    

    return GFEC_NOT_FOUND;
}

static size_t allocUnitAlignedBytes(const uint_8 * const headerBytes,
                                    const size_t bytes) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    return aligned(bytes, bpb->bytesPerLogicalSector * bpb->logicalSectorsPerCluster);
}

static bool detect(const uint_8 * const headerBytes,
                   const struct gnwStorGeometry geometry) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    if (__builtin_popcount(bpb->bytesPerLogicalSector) != 1) {
        return false;
    }
    
    if (bpb->totalLogicalSectors > geometry.lba) {
        return false;
    }
    if (geometry.sectSizeBytes != bpb->bytesPerLogicalSector) {
        return false;
    }

    if (!bpb->logicalSectorsPerCluster) {
        return false;
    }
    if (!bpb->reservedLogicalSectors) {
        return false;
    }
    if (!bpb->numberOfFATs) {
        return false;
    }
    if (!bpb->maxRootDirectoryEntries) {
        return false;
    }
    if (!bpb->totalLogicalSectors) {
        return false;
    }
    if (bpb->mediaDescriptor._reserved != 0xF) {
        return false;
    }
    if (!bpb->logicalSectorsPerFAT) {
        return false;
    }
    if (!bpb->physicalSectorsPerTrack) {
        return false;
    }
    if (!bpb->numberOfHeads) {
        return false;
    }
    if (!bpb->numberOfHeads) {
        return false;
    }
    if (!bpb->totalLogicalSectors) {
        return false;
    }
    if (bpb->extendedBootSignature != EXTENDED_BOOT_SIGNATURE) {
        return false;
    }
    
    if (bpb->totalLogicalSectors % bpb->logicalSectorsPerCluster) {
        return false;
    }

    const size_t directoryBytes = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);
    if (directoryBytes % bpb->bytesPerLogicalSector) {
        return false;
    }

    if (bpb->hiddenSectors >= bpb->totalLogicalSectors) {
        return false;
    }

    const size_t utilitySectors = bpb->reservedLogicalSectors +
                                  bpb->numberOfFATs * bpb->logicalSectorsPerFAT +
                                  directoryBytes / bpb->bytesPerLogicalSector;
    if (utilitySectors >= (bpb->totalLogicalSectors - bpb->hiddenSectors)) {
        return false;
    }
    
    const size_t dataSectors = bpb->totalLogicalSectors - utilitySectors - bpb->hiddenSectors;
    if (dataSectors % bpb->logicalSectorsPerCluster) {
        return false;
    }

    const size_t dataClusters = dataSectors / bpb->logicalSectorsPerCluster;
    if (bpb->maxRootDirectoryEntries > dataClusters) {
        return false;
    }

    const size_t fatBytes = bpb->logicalSectorsPerFAT * bpb->bytesPerLogicalSector;
    if (fatBytes * 8 % BITS_PER_FAT_ENTRY) {
        return false;
    }
    const size_t fatEntries = BYTES_TO_FAT_ENTRIES(fatBytes) - 2;
    if (fatEntries < dataClusters) {
        return false;
    }

    return !strcmpl((char *)bpb->fileSystemType, FILE_SYSTEM_NAME, FILE_SYSTEM_NAME_BYTES);
}

struct gnwFileSystemDescriptor k_drv_fat12_descriptor() {
    return (struct gnwFileSystemDescriptor) {
        /* headerRange */ (range_size_t) {
            /* offset */ 0x0B,
            /* size */ 51,
        },
        /* maxFilenameLength */ FILE_NAME_MAX_LENGTH,
        /* maxExtensionLength */ FILE_EXTENSION_MAX_LENGTH,
        /* directoryRange */ directoryRange,
        /* fatRange */ fatRange,
        /* fatVerify */ fatVerify,
        /* fileStartLocation */ fileStartLocation,
        /* nextLocation */ nextLocation,
        /* isValidForRead */ isValidForRead,
        /* isEOF */ isEOF,
        /* fileInfo */ fileInfo,
        /* allocUnitAlignedBytes */ allocUnitAlignedBytes,
        /* detect */ detect
    };
}
