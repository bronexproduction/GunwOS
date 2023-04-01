//
//  fat12.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include <driver/gunwfilesys.h>
#include <string.h>
#include <gunwstor.h>
#include "model.h"

#define FILE_SYSTEM_NAME_HEADER_OFFSET 0x2B
#define FILE_SYSTEM_NAME "FAT12   "
#define FILE_SYSTEM_NAME_BYTES 8
#define FILE_NAME_MAX_LENGTH 8
#define FILE_EXTENSION_MAX_LENGTH 3
#define MIN_FILE_CLUSTER 2

static range_size_t directoryRange(const uint_8 * const headerBytes) {
    range_size_t range;

    if (!headerBytes) {
        range.offset = 0;
        range.length = 0;
        return range;
    }

    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    range.offset = (bpb->reservedLogicalSectors + (bpb->numberOfFATs * bpb->logicalSectorsPerFAT)) * bpb->bytesPerLogicalSector;
    range.length = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);

    return range;
}

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

static bool validateDirEntry(const struct dos_4_0_ebpb_t * const bpb,
                             const struct fat12_dir_t * const entry) {
    if (entry->firstLogicalCluster < MIN_FILE_CLUSTER) {
        return false;
    }
    const size_t fatSectors = bpb->numberOfFATs * bpb->logicalSectorsPerFAT;
    const size_t dirBytes = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);
    if (dirBytes % (bpb->bytesPerLogicalSector * bpb->logicalSectorsPerCluster)) {
        #warning invalid directory structure - shoule be checked on install
        return false;
    }
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

    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;

    for (size_t dirIndex = 0; dirIndex < bpb->maxRootDirectoryEntries; ++dirIndex) {
        const struct fat12_dir_t * const dirEntry = (struct fat12_dir_t *)(directoryBytes + dirIndex * sizeof(struct fat12_dir_t));
        if (!strcmpl(name, (char *)dirEntry->filename, FILE_NAME_MAX_LENGTH) &&
            !strcmpl(extension, (char *)dirEntry->extension, FILE_EXTENSION_MAX_LENGTH)) {
            if (!validateDirEntry(bpb, dirEntry)) {
                return GFEC_NOT_FOUND;
            }

            fileInfo->sizeBytes = dirEntry->fileSizeBytes;

            return GFEC_NONE;
        }
    }    

    return GFEC_NOT_FOUND;
}

static bool detect(const uint_8 * const header) {
    #warning to be improved?
    return !strcmpl((char *)header + FILE_SYSTEM_NAME_HEADER_OFFSET, FILE_SYSTEM_NAME, FILE_SYSTEM_NAME_BYTES);
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
        /* fileInfo */ fileInfo,
        /* detect */ detect
    };
}
