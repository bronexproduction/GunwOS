//  gunwfug.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWFUG_H
#define GUNWOS_GUNWFUG_H

#include "scl_def.h"

#define CHECKPTR(PTR) { if (!PTR) { fug(NULLPTR); __builtin_unreachable(); } }

enum gnwFugCode {
    NULLPTR
};

/*
    Sends FUG command to the kernel

    FUG command informs about some kind of terrible failure
    happened inside the caller process
    and it should be terminated
*/
SYSCALL_DECL void fug(enum gnwFugCode code) {
    SYSCALL_PAR1(code);

    SYSCALL_USER_FUNC(FUG);
    SYSCALL_USER_INT;
}

#endif // GUNWOS_GUNWFUG_H
