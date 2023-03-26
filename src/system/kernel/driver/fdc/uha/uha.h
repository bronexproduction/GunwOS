//
//  uha.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#ifndef UHA_H
#define UHA_H

#include <gunwstor.h>

bool uha_drivePresent(const uint_8 index);
struct gnwStorGeometry uha_driveGeometry(const uint_8 index);
size_t uha_read(const uint_8 index,
                const size_t lba,
                const size_t count,
                uint_8 * const buffer,
                struct gnwStorError * const error);

#endif // UHA_H
