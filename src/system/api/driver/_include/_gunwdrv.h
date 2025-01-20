//
//  _gunwdrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#ifndef GUNWOS__GUNWDRV_H
#define GUNWOS__GUNWDRV_H

#ifdef _GUNWAPI_KERNEL

#include "../include/gunwdrv.h"
#include <_gunwdev.h>

bool validateDeviceDescriptor(const struct gnwDeviceDescriptor * const);

#endif // GUNWOS__GUNWDRV_H

#endif // _GUNWAPI_KERNEL
