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

    Note:
        * Not allowed from user-level
*/
extern enum gnwDeviceError emit(const struct gnwDeviceEvent * const event);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWDEVEMITTER_H
