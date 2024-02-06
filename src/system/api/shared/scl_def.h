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

#define REG_RET(BITS, NAME) REG(BITS, NAME, eax)
#define SYSCALL_RETVAL(BITS) { REG_RET(BITS, _retVal); return _retVal; }
#define SYSCALL_GET_RETVAL(BITS, NAME) REG_RET(BITS, _retVal); uint_ ## BITS NAME = _retVal;

/*
    Important note:

    When setting syscall parameters
    remember that
    SYSCALL_PAR* must be called before SYSCALL_FUNC
    as EAX register is used as intermediate register
    and stores the last assigned value
*/
#define _SYSCALL_FUNC(CODE) REG(32, _code, eax); _code = CODE;
#define _SYSCALL_INT(CODE) { __asm__ volatile ("int $" STR(CODE) ); }
#define _SYSCALL_PAR(NAME, VALUE, REG_NAME) REG(32, _param ## NAME , REG_NAME); _param ## NAME = (uint_32) VALUE ;
#define _SYSCALL_PAR1(VALUE) _SYSCALL_PAR(1, VALUE, ebx)
#define _SYSCALL_PAR2(VALUE) _SYSCALL_PAR(2, VALUE, ecx)
#define _SYSCALL_PAR3(VALUE) _SYSCALL_PAR(3, VALUE, edx)

#define SYSCALL_CALL(INT_CODE, CODE, PAR1, PAR2, PAR3) {\
    _SYSCALL_PAR1(PAR1);                                \
    _SYSCALL_PAR2(PAR2);                                \
    _SYSCALL_PAR3(PAR3);                                \
    _SYSCALL_FUNC(CODE);                                \
    _SYSCALL_INT(INT_CODE);                             \
}

#endif // GUNWOS_SCL_DEF_H
