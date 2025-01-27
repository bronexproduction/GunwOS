//
//  gunwfug.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwfug.h"
#include "../_include/scl_user.h"

void fug(enum gnwFugCode code) {
    SYSCALL_USER_CALL(FUG, code, 0, 0);
}

#endif // _GUNWAPI_KERNEL
