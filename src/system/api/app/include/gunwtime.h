//
//  gunwtime.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2023.
//

#ifndef GUNWOS_GUNWTIME_H
#define GUNWOS_GUNWTIME_H

#include "scl_def.h"

/*
    Returns kernel time in milliseconds
*/
SYSCALL_DECL time_t timeMs() {
    SYSCALL_USER_FUNC(TIME_MS);
    SYSCALL_USER_INT;
    
    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWTIME_H
