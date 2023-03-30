//
//  filesys.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef FILESYS_H
#define FILESYS_H

#include <driver/gunwfilesys.h>
#include "storage.h"

#define MAX_FILESYS 2

extern struct k_stor_fileSystem {
    /*
        Entry status
    */
    bool used;

    /*
        File system descriptor
    */
    struct gnwFileSystemDescriptor desc;
} k_stor_fileSystems[MAX_FILESYS];

enum k_stor_error k_stor_fileSys_install(const struct gnwFileSystemDescriptor * const);

bool k_stor_fileSys_validateId(const size_t fileSysId);

#endif // FILESYS_H
