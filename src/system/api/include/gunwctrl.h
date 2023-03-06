//
//  gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_GUNWCTRL_H
#define GUNWOS_GUNWCTRL_H

#include "scl_def.h"

/*
    Ends process execution
*/
SYSCALL_DECL void exit(uint_32 const status) {
    SYSCALL_PAR1(status);

    SYSCALL_USER_FUNC(EXIT);
    SYSCALL_USER_INT;
}

/*
    Performs wait for given milliseconds

    Params:
        * ms - wait length in milliseconds
*/
SYSCALL_DECL void sleepms(const unsigned int ms) {
    SYSCALL_PAR1(ms);

    SYSCALL_USER_FUNC(SLEEPMS);
    SYSCALL_USER_INT;
}

#endif // GUNWOS_GUNWCTRL_H
