//
//  _gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifndef GUNWOS__GUNWDEV_H
#define GUNWOS__GUNWDEV_H

#include "../include/gunwdev.h"

#ifdef _GUNWAPI_KERNEL

void gnwDeviceEvent_encode(const struct gnwDeviceEvent * const, ptr_t);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS__GUNWDEV_H
