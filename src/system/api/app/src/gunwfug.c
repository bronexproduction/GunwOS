//
//  gunwfug.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#warning commented until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL

#include "../include/gunwfug.h"
#include "scl_user.h"

void fug(enum gnwFugCode code) {
    SYSCALL_PAR1(code);

    SYSCALL_USER_FUNC(FUG);
    SYSCALL_USER_INT;
}

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL
