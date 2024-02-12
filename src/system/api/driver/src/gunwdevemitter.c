//
//  gunwdevemitter.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwdevemitter.h"
#include <gunwfug.h>
#include "scl_driver.h"

enum gnwDeviceError emit(const struct gnwDeviceEvent * const event) {
    CHECKPTR(event);
    CHECKPTR(event->data);

    SYSCALL_DRIVER_CALL(EMIT, event, 0, 0);
    
    SYSCALL_RETVAL(32);
}

#endif // _GUNWAPI_KERNEL
