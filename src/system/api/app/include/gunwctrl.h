//
//  gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_GUNWCTRL_H
#define GUNWOS_GUNWCTRL_H

#include <string.h>

#include "gunwfug.h"

enum gnwCtrlError {
    GCE_NONE = 0,
    GCE_INVALID_ARGUMENT,
    GCE_HEADER_INVALID,
    GCE_NOT_FOUND,
    GCE_OPERATION_FAILED,
    GCE_UNKNOWN
};

/*
    Starts program

    Params:
    * path - path to the executable
*/
SYSCALL_DECL enum gnwCtrlError start(const char * const path) {
    CHECKPTR(path);

    SYSCALL_PAR1(path);
    SYSCALL_PAR2(strlen(path));

    SYSCALL_USER_FUNC(START);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Ends process execution
*/
SYSCALL_DECL void exit(const int_32 status) {
    SYSCALL_PAR1(status);

    SYSCALL_USER_FUNC(EXIT);
    SYSCALL_USER_INT;
}

/*
    Suspends process execution until an event is received
*/
SYSCALL_DECL void waitForEvent() {
    SYSCALL_USER_FUNC(WAIT_FOR_EVENT);
    SYSCALL_USER_INT;
}

#endif // GUNWOS_GUNWCTRL_H
