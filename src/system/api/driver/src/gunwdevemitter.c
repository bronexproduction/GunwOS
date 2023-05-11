//
//  gunwdevemitter.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#warning commented until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL

#include "../include/gunwdevemitter.h"
#include <gunwfug.h>
#include "scl_driver.h"

enum gnwDeviceError emit(const struct gnwDeviceEvent * const event) {
    CHECKPTR(event);
    CHECKPTR(event->data);

    SYSCALL_PAR1(event);

    SYSCALL_DRIVER_FUNC(EMIT);
    SYSCALL_DRIVER_INT;
    
    SYSCALL_RETVAL(32);
}

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL