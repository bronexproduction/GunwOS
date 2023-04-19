//
//  gunwdisplaydrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_GUNWDISPLAYDRV_H
#define GUNWOS_GUNWDISPLAYDRV_H

#include <gunwdrv.h>

extern bool uhaGetParam(const size_t param,
                        const size_t subParam,
                        const size_t paramIndex,
                        size_t * const result);

#endif // GUNWOS_GUNWDISPLAYDRV_H
