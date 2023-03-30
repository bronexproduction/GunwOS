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
    SE_NONE = 0,
    SE_LIMIT_REACHED,
    SE_UNKNOWN
};

#endif // STORAGE_H
