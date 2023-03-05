//
//  gunwio.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef GUNWOS_GUNWIO_H
#define GUNWOS_GUNWIO_H

#include "scl_def.h"

/*
    Print string to debug output

    Parameters:
    * buffer - zero-terminated character array
    Return value: number of bytes written or -1 on error
*/
SYSCALL_DECL int_32 debugPrint(const char * const buffer) {
    SYSCALL_PAR1(buffer);

    SYSCALL_USER_FUNC(DEBUG_PRINT);
    SYSCALL_USER_INT;
    
    register int_32 ret __asm__ ("eax");
    return ret;
}

#endif // GUNWOS_GUNWIO_H
