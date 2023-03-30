//
//  file.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "file.h"
#include <storage/storage.h>
#include <storage/data.h>
#include <storage/volume.h>
#include <error/panic.h>

enum gnwFileErrorCode k_stor_file_getInfo(const char * const path, 
                                          const size_t pathLen, 
                                          struct gnwStorFileInfo * const fileInfo) {
    if (!path) {
        OOPS("Nullptr access");
        return GFEC_UNKNOWN;
    }

    /*
        Get volume ID from path

        TODO: Not implemented yet
    */

    size_t volumeId = 0;
    if (volumeId >= MAX_VOLUMES) {
        return GFEC_INVALID_PATH;
    }
    if (!volumes[volumeId].used) {
        return GFEC_INVALID_PATH;
    }
    size_t fileSysId = volumes[volumeId].fileSysId;
    if (fileSysId >= MAX_FILESYS) {
        OOPS("Invalid FSID");
        return GFEC_UNKNOWN;
    }
    if (!fileSys[fileSysId].used) {
        OOPS("Unused FSID");
        return GFEC_UNKNOWN;
    }

    struct gnwFileSystemDescriptor fsDesc = fileSys[fileSysId].desc;
    if (!fsDesc.headerRange.offset ||
        !fsDesc.directoryRange) {
        OOPS("Invalid FSDESC");
        return GFEC_UNKNOWN;
    }

    uint_8 headerBytes[fsDesc.headerRange.length];
    enum k_stor_error err = k_stor_vol_readHeader(0, headerBytes);
    if (err != SE_NONE) {
        return GFEC_UNKNOWN;
    }

    return GFEC_NONE;
}
