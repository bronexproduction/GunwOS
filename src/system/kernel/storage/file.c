//
//  file.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "file.h"
#include "storage.h"
#include "volume.h"
#include "filesys.h"
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
    if (!k_stor_volume_validateId(volumeId)) {
        return GFEC_INVALID_PATH;
    }
    size_t fileSysId = k_stor_volumes[volumeId].fileSysId;
    struct gnwFileSystemDescriptor fsDesc = k_stor_fileSystems[fileSysId].desc;
    uint_8 headerBytes[fsDesc.headerRange.length];
    enum k_stor_error err = k_stor_volume_readHeader(0, headerBytes);
    if (err != SE_NONE) {
        return GFEC_UNKNOWN;
    }

    return GFEC_NONE;
}
