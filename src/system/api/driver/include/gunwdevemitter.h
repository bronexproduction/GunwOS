//
//  gunwdevemitter.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWDEVEMITTER_H
#define GUNWOS_GUNWDEVEMITTER_H

#include <types.h>
#include <scl_driver.h>
#include <gunwdev.h>

/*
    Emit device event

    Params:
        * event - event descriptor (see struct gnwDeviceEvent)

    Note:
        * Not allowed from user-level
*/
SYSCALL_DECL enum gnwDeviceError emit(const struct gnwDeviceEvent * const event) {
    CHECKPTR(event);
    CHECKPTR(event->data);

    SYSCALL_PAR1(event);

    SYSCALL_DRIVER_FUNC(EMIT);
    SYSCALL_DRIVER_INT;
    
    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWDEVEMITTER_H
