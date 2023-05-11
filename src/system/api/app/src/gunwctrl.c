//
//  gunwctrl.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwctrl.h"
#include <string.h>
#include "scl_user.h"

enum gnwCtrlError start(const char * const path) {
    CHECKPTR(path);

    SYSCALL_PAR1(path);
    SYSCALL_PAR2(strlen(path));

    SYSCALL_USER_FUNC(START);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

void exit(const int_32 status) {
    SYSCALL_PAR1(status);

    SYSCALL_USER_FUNC(EXIT);
    SYSCALL_USER_INT;
}

void waitForEvent() {
    SYSCALL_USER_FUNC(WAIT_FOR_EVENT);
    SYSCALL_USER_INT;
}

extern void dupa();

void _start() {
    dupa();
    exit(0);
}

#endif // _GUNWAPI_KERNEL
