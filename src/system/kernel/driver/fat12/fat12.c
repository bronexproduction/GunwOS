//
//  fat12.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#include <driver/gunwfilesys.h>

static bool detect(const uint_8 * const header) {
    return false;
}

struct gnwFileSystemDescriptor k_drv_fat12_descriptor() {
    return (struct gnwFileSystemDescriptor) {
        /* headerOffset */ 0x0B,
        /* headerSize */ 51,
        /* detect */ detect
    };
}
