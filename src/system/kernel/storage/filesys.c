//
//  filesys.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#include "filesys.h"
#include "storage.h"
#include <error/panic.h>

struct k_stor_fileSystem k_stor_fileSystems[MAX_FILESYS];

enum k_stor_error k_stor_fileSys_install(const struct gnwFileSystemDescriptor * const desc) {
    size_t fileSysIndex = 0;
    for (; fileSysIndex < MAX_FILESYS; ++fileSysIndex) {
        if (!k_stor_fileSystems[fileSysIndex].used) {
            break;
        }
    }
    if (fileSysIndex >= MAX_FILESYS) {
        OOPS("File system count limit exceeded", SE_LIMIT_REACHED);
    }
            
    k_stor_fileSystems[fileSysIndex].used = true;
    k_stor_fileSystems[fileSysIndex].desc = *desc;
    
    return SE_NONE;
}

bool k_stor_fileSys_validateId(const size_t fileSysId) {
    if (fileSysId >= MAX_FILESYS) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].used) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.headerRange.length) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.maxFilenameLength) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.maxExtensionLength) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.sectorsPerAllocUnit) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.directoryRange) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.fatRange) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.fatVerify) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.fileStartLocation) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.nextLocation) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.isContiguous) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.isValidForRead) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.isEOF) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.fileInfo) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.allocUnitAlignedBytes) {
        return false;
    }
    if (!k_stor_fileSystems[fileSysId].desc.detect) {
        return false;
    }
    
    return true;
}
