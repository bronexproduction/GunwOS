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
uint_8 rdb(uint_16 const port) {
    SYSCALL_DRIVER_CALL(RDB, port, 0, 0);
    
    return SYSCALL_RESULT;
}

/*
    Write byte to system bus
*/
void wrb(uint_16 const port, uint_8 const value) {
    SYSCALL_DRIVER_CALL(WRB, port, value, 0);
}

#endif // _GUNWAPI_KERNEL
