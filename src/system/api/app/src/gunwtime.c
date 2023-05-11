//
//  gunwtime.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwtime.h"
#include "scl_user.h"

time_t timeMs() {
    SYSCALL_USER_FUNC(TIME_MS);
    SYSCALL_USER_INT;
    
    SYSCALL_RETVAL(32);
}

#endif // _GUNWAPI_KERNEL
