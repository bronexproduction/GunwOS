//
//  gunwbus.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwbus.h"
#include <scl_driver.h>

/*
    Read byte from system bus
*/
uint_8 rdb(const size_t deviceId, const uint_16 port) {
    SYSCALL_DRIVER_CALL(RDB, deviceId, port, 0, 0);
    
    return SYSCALL_RESULT;
}

/*
    Write byte to system bus
*/
void wrb(const size_t deviceId, const uint_16 port, const uint_8 value) {
    SYSCALL_DRIVER_CALL(WRB, deviceId, port, value, 0);
}

#endif // _GUNWAPI_KERNEL
