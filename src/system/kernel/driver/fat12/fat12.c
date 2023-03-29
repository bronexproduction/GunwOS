//
//  fat12.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include <driver/gunwfilesys.h>
#include <string.h>

#define FILE_SYSTEM_NAME_HEADER_OFFSET 0x2B
#define FILE_SYSTEM_NAME "FAT12   "
#define FILE_SYSTEM_NAME_BYTES 8

static bool detect(const uint_8 * const header) {
    #warning to be improved?
    return !strcmpl((char *)header + FILE_SYSTEM_NAME_HEADER_OFFSET, FILE_SYSTEM_NAME, FILE_SYSTEM_NAME_BYTES);
}

struct gnwFileSystemDescriptor k_drv_fat12_descriptor() {
    return (struct gnwFileSystemDescriptor) {
        /* headerOffset */ 0x0B,
        /* headerSize */ 51,
        /* detect */ detect
    };
}
