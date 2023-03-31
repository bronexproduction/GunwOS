//
//  fat12.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include <driver/gunwfilesys.h>
#include <string.h>
#include "model.h"

#define FILE_SYSTEM_NAME_HEADER_OFFSET 0x2B
#define FILE_SYSTEM_NAME "FAT12   "
#define FILE_SYSTEM_NAME_BYTES 8

static range_size_t directoryRange(const uint_8 * const headerBytes) {
    const struct dos_4_0_ebpb_t * const bpb = (struct dos_4_0_ebpb_t *)headerBytes;
    range_size_t range;

    range.offset = (bpb->reservedLogicalSectors + (bpb->numberOfFATs * bpb->logicalSectorsPerFAT)) * bpb->bytesPerLogicalSector;
    range.length = bpb->maxRootDirectoryEntries * sizeof(struct fat12_dir_t);

    return range;
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
        /* directoryRange */ directoryRange,
        /* fileInfo */ fileInfo,
        /* detect */ detect
    };
}
