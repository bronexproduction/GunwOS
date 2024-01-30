//
//  gunwmem.c
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2024.
//

#include "../include/gunwmem.h"
#include "../include/gunwfug.h"
#include "scl_user.h"

#ifndef _GUNWAPI_KERNEL

enum gnwMemoryError memplz(const size_t pageCount, addr_t * const start) {
    CHECKPTR(start);

    SYSCALL_PAR1(pageCount);
    SYSCALL_PAR2(start);

    SYSCALL_USER_FUNC(MEM_PLZ);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwMemoryError memthx(const addr_t start) {
    SYSCALL_PAR1(start);

    SYSCALL_USER_FUNC(MEM_THX);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // _GUNWAPI_KERNEL
