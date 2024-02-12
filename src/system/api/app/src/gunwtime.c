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
    SYSCALL_USER_CALL(TIME_MS, 0, 0, 0);
    
    SYSCALL_RETVAL(32);
}

#endif // _GUNWAPI_KERNEL
