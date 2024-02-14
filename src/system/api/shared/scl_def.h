//
//  scl_def.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_SCL_DEF_H
#define GUNWOS_SCL_DEF_H

#include <types.h>
#include <utils.h>

#define _SYSCALL_PUSH_PAR(PAR) { int_32 par = (int_32)PAR; __asm__ volatile ("pushl %[mem]" : : [mem] "m" (par)); }
#define _SYSCALL_INT(CODE) { __asm__ volatile ("int $" STR(CODE) ); }
#define _SYSCALL_REMOVE_PAR { __asm__ volatile ("addl $16, %esp"); }

#define SYSCALL_CALL(INT_CODE, CODE, PAR1, PAR2, PAR3) {\
    _SYSCALL_PUSH_PAR(PAR3);                            \
    _SYSCALL_PUSH_PAR(PAR2);                            \
    _SYSCALL_PUSH_PAR(PAR1);                            \
    _SYSCALL_PUSH_PAR(CODE);                            \
    _SYSCALL_INT(INT_CODE);                             \
    _SYSCALL_REMOVE_PAR;                                \
}

#define SYSCALL_RESULT 0

#endif // GUNWOS_SCL_DEF_H
