//
//  gunwctrl.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwctrl.h"
#include "../include/gunwfug.h"
#include <string.h>
#include "scl_user.h"

void start(const char * const path, enum gnwCtrlError * const error) {
    CHECKPTR(path);
    CHECKPTR(error);

    SYSCALL_PAR1(path);
    SYSCALL_PAR2(strlen(path));
    SYSCALL_PAR3(error);

    SYSCALL_USER_FUNC(START);
    SYSCALL_USER_INT;
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

#endif // _GUNWAPI_KERNEL
