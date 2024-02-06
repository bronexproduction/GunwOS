//
//  gunwtime.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#warning commented until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL

#include "../include/gunwtime.h"
#include "scl_user.h"

time_t timeMs() {
    SYSCALL_USER_CALL(TIME_MS, 0, 0, 0);
    
    SYSCALL_RETVAL(32);
}

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL
