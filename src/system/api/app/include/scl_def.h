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

/*
    Important note:

    When setting syscall parameters
    remember that
    SYSCALL_PAR* must be called before SYSCALL_FUNC
    as EAX register is used as intermediate register
    and stores the last assigned value
*/
#define SYSCALL_DECL inline
#define SYSCALL_FUNC(LEVEL, CODE) REG(32, _code, eax); _code = (uint_32) SYSCALL_ ## LEVEL ## _FUNCTION_ ## CODE ;
#define SYSCALL_INT(CODE) { __asm__ volatile ("int $" STR(CODE) ); }
#define REG_RET(BITS, NAME) REG(BITS, NAME, eax)
#define SYSCALL_RETVAL(BITS) { REG_RET(BITS, _retVal); return _retVal; }

#define SYSCALL_PAR(NAME, VALUE, REG_NAME) REG(32, _param ## NAME , REG_NAME); _param ## NAME = (uint_32) VALUE ;
#define SYSCALL_PAR1(VALUE) SYSCALL_PAR(1, VALUE, ebx)
#define SYSCALL_PAR2(VALUE) SYSCALL_PAR(2, VALUE, ecx)
#define SYSCALL_PAR3(VALUE) SYSCALL_PAR(3, VALUE, edx)

#endif // GUNWOS_SCL_DEF_H
