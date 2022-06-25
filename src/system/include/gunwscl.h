//
//  gunwscl.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_GUNWSCL_H
#define GUNWOS_GUNWSCL_H

#include "scl_def.h"

/*
    Ends process execution
*/
static inline void exit(uint_32 const status) {
    SYSCALL_PAR1(status);

    SYSCALL_FUNC(EXIT);
    SYSCALL_INT;
}

/*
    Adds function call described in gnwDispatchDesc to global run loop

    Params:
        * desc - pointer to struct gnwDispatchDesc instance
*/
static inline void dispatch(const struct gnwDispatchDesc * const desc) {
    SYSCALL_PAR1(desc);

    SYSCALL_FUNC(DISPATCH);
    SYSCALL_INT;
}

/*
    Performs wait for given milliseconds

    Params:
        * ms - wait length in milliseconds
*/
static inline void sleepms(const unsigned int ms) {
    SYSCALL_PAR1(ms);

    SYSCALL_FUNC(SLEEPMS);
    SYSCALL_INT;
}

#endif // GUNWOS_GUNWSCL_H
