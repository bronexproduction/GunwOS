//
//  storage.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.03.2023.
//

#ifndef STORAGE_H
#define STORAGE_H

#include <gunwfilesys.h>

enum k_stor_error {
    SE_NONE = 0,
    SE_LIMIT_REACHED,
    SE_INVALID_DEVICE,
    SE_UNKNOWN
};

void k_stor_init();

#endif // STORAGE_H
