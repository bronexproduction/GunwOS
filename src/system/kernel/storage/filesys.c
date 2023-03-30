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
        OOPS("File system count limit exceeded");
        return SE_LIMIT_REACHED;
    }
            
    k_stor_fileSystems[fileSysIndex].used = true;
    k_stor_fileSystems[fileSysIndex].desc = *desc;
    return SE_NONE;
}

bool k_stor_fileSys_validateId(const size_t fileSysId) {
    #warning TO BE IMPLEMENTED
    return false;
}
