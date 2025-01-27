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
extern void uhaSetParam_display(const struct gnwDeviceSetParamQuery * const query);

#endif // _GUNWAPI_KERNEL             

#endif // GUNWOS_GUNWDISPLAYDRV_H
