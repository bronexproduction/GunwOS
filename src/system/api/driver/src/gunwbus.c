//
//  gunwbus.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#warning swapped until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL
#ifdef _GUNWAPI_KERNEL

#include "../include/gunwbus.h"
#include "scl_driver.h"

/*
    Read byte from system bus

    Note:
        * Not allowed from user-level
*/
uint_8 rdb(uint_16 const port) {
    SYSCALL_PAR1(port);

#warning TODO: check if the driver is allowed to access the requested port (driver descriptor)

    SYSCALL_DRIVER_FUNC(RDB);
    SYSCALL_DRIVER_INT;
    
    register uint_8 ret __asm__ ("al");
    return ret;
}

/*
    Write byte to system bus

    Note:
        * Not allowed from user-level
*/
void wrb(uint_16 const port, uint_8 const val) {
    SYSCALL_PAR1(port);
    SYSCALL_PAR2(val);

#warning TODO: check if the driver is allowed to access the requested port (driver descriptor)

    SYSCALL_DRIVER_FUNC(WRB);
    SYSCALL_DRIVER_INT;
}

#endif // _GUNWAPI_KERNEL
