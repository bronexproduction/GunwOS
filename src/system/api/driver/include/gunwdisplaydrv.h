//
//  gunwdisplaydrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_GUNWDISPLAYDRV_H
#define GUNWOS_GUNWDISPLAYDRV_H

#include <gunwdrv.h>

extern bool uhaGetParam_display(const size_t paramVal,
                                const size_t subParamVal,
                                const size_t paramIndex,
                                size_t * const result);
extern bool uhaSetParam_display(const size_t paramVal,
                                const size_t subParamVal,
                                const size_t paramIndex,
                                const size_t value);

#endif // GUNWOS_GUNWDISPLAYDRV_H
