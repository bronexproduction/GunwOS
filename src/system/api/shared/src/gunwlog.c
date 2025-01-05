//
//  gunwlog.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwlog.h"
#include "../_include/scl_user.h"
#include <string.h>

void log(const char * const msg) {
    SYSCALL_USER_CALL(LOG, msg, strlen(msg), 0);
}

#endif // _GUNWAPI_KERNEL
