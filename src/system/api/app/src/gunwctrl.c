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

    SYSCALL_USER_CALL(START, &desc, 0, 0);
}

void bye(const int_32 status) {
    SYSCALL_USER_CALL(BYE, status, 0, 0);
}

void waitForEvent() {
    SYSCALL_USER_CALL(WAIT_FOR_EVENT, 0, 0, 0);
}

void yield() {
    SYSCALL_USER_CALL(YIELD, 0, 0, 0);
}

#endif // _GUNWAPI_KERNEL
