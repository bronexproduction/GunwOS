//
//  gunwio.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.02.2021.
//

#ifndef GUNWOS_GUNWIO_H
#define GUNWOS_GUNWIO_H

#include "scl_def.h"

/*
    Read byte from system bus

    Note:
        * Not allowed from user-level
*/
static inline uint_8 rdb(uint_16 const port) {
    SYSCALL_PAR1(port);

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
static inline void wrb(uint_16 const port, uint_8 const val) {
    SYSCALL_PAR1(port);
    SYSCALL_PAR2(val);

    SYSCALL_DRIVER_FUNC(WRB);
    SYSCALL_DRIVER_INT;
}

#endif // GUNWOS_GUNWIO_H
