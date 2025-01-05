//
//  gunwctrl.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../_include/_gunwctrl.h"
#include "../_include/scl_user.h"
#include "../include/gunwfug.h"
#include <string.h>

procId_t start(const char * const path) {
    CHECKPTR(path);

    procId_t procId;
    struct gnwCtrlStartDescriptor desc = { { (byte_t *)path, strlen(path) }, GET_PROGRAM, &procId }; 

    SYSCALL_USER_CALL(START, &desc, 0, 0);

    return procId;
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
