//
//  gunwctrl.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwctrl.h"
#include "_gunwctrl.h"
#include "../include/gunwfug.h"
#include <string.h>
#include "scl_user.h"

void start(const char * const path, enum gnwCtrlError * const error) {
    CHECKPTR(path);
    CHECKPTR(error);

    struct gnwCtrlStartDescriptor desc;
    desc.pathPtr = path;
    desc.pathLen = strlen(path);
    desc.errorPtr = error;

    SYSCALL_PAR1(&desc);

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
