//
//  gunwdevemitter.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWDEVEMITTER_H
#define GUNWOS_GUNWDEVEMITTER_H

#ifndef _GUNWAPI_KERNEL

#include <gunwdev.h>

/*
    Emit device event

    Params:
        * event - event descriptor (see struct gnwDeviceEvent)
*/
enum gnwDeviceError emit(const size_t deviceId, const struct gnwDeviceEvent * const event);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWDEVEMITTER_H
