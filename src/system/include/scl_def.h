//
//  scl_def.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_SCL_DEF_H
#define GUNWOS_SCL_DEF_H

#include "../../lib/stdgunw/types.h"
#include "../../lib/stdgunw/utils.h"

#define SYSCALL_INTERRUPT           0x69

#define SYSCALL_FUNCTION_RDB        0x01
#define SYSCALL_FUNCTION_WRB        0x02
#define SYSCALL_FUNCTION_EXIT       0x03
#define SYSCALL_FUNCTION_PRINTL     0x04
#define SYSCALL_FUNCTION_DISPATCH   0x05
#define SYSCALL_FUNCTION_SLEEPMS    0x06

/*
    Important note:

    When setting syscall parameters
    remember that
    SYSCALL_PAR* must be called before SYSCALL_FUNC
    as EAX register is used as intermediate register
    and stores the last assigned value
*/
#define SYSCALL_FUNC(CODE) REG(32, _code, eax); _code = (uint_32) SYSCALL_FUNCTION_ ## CODE ;
#define SYSCALL_INT { __asm__ volatile ("int $" STR(SYSCALL_INTERRUPT) ); }

#define SYSCALL_PAR(NAME, VALUE, REG_NAME) REG(32, _param ## NAME , REG_NAME); _param ## NAME = (uint_32) VALUE ;
#define SYSCALL_PAR1(VALUE) SYSCALL_PAR(1, VALUE, ebx)
#define SYSCALL_PAR2(VALUE) SYSCALL_PAR(2, VALUE, ecx)

#endif // GUNWOS_SCL_DEF_H
