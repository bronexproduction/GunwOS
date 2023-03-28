//
//  storage.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#ifndef STORAGE_H
#define STORAGE_H

#include <driver/gunwfilesys.h>

enum k_stor_error {
    SE_NONE = 0
};

enum k_stor_error k_stor_fileSysInstall(const struct gnwFileSystemDescriptor * const);

#endif // STORAGE_H
