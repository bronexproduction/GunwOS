//  gunwfug.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWFUG_H
#define GUNWOS_GUNWFUG_H

#include "scl_def.h"

enum gnwFugCode {
    NULLPTR
};

/*
    Sends FUG command to the kernel

    FUG command informs about some kind of terrible failure
    happened inside the caller process
    and it should be terminated
*/
static inline void fug(enum gnwFugCode code) {
    SYSCALL_PAR1(code);

    SYSCALL_FUNC(FUG);
    SYSCALL_INT;
}

#endif // GUNWOS_GUNWFUG_H
