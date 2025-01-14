//
//  gunwdisplaydrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_GUNWDISPLAYDRV_H
#define GUNWOS_GUNWDISPLAYDRV_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>

extern void uhaGetParam_display(const struct gnwDeviceGetParamQuery * const query);
extern bool uhaSetParam_display(const size_t paramVal,
                                const size_t subParamVal,
                                const size_t paramIndex,
                                const size_t value);

#endif // _GUNWAPI_KERNEL             

#endif // GUNWOS_GUNWDISPLAYDRV_H
