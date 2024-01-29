//
//  gunwlog.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwlog.h"
#include "scl_user.h"
#include <string.h>

void log(const char * const msg) {
    SYSCALL_PAR1(msg);
    SYSCALL_PAR2(strlen(msg));

    SYSCALL_USER_FUNC(LOG);
    SYSCALL_USER_INT;
}

#endif // _GUNWAPI_KERNEL
