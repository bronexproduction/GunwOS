//
//  _gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifdef _GUNWAPI_KERNEL

#ifndef GUNWOS__GUNWDEV_H
#define GUNWOS__GUNWDEV_H

#include <gunwdev.h>

void gnwDeviceEvent_encode(const struct gnwDeviceEvent * const, ptr_t);
void gnwDeviceEvent_decode(const ptr_t, struct gnwDeviceEvent * const);

#endif // GUNWOS__GUNWDISPLAY_H

#endif // _GUNWAPI_KERNEL
