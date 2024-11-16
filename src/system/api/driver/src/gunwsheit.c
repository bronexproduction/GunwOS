//
//  gunwsheit.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.11.2024.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwsheit.h"
#include "scl_driver.h"

void sheit(enum gnwSheitCode code) {
    SYSCALL_DRIVER_CALL(SHEIT, code, 0, 0);
}

#endif // _GUNWAPI_KERNEL
