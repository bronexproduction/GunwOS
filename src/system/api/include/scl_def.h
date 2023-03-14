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

#define SYSCALL_DRIVER_INTERRUPT                0x45
#define SYSCALL_USER_INTERRUPT                  0x69

#define SYSCALL_DRIVER_FUNCTION_RDB             0x00
#define SYSCALL_DRIVER_FUNCTION_WRB             0x01
#define SYSCALL_DRIVER_FUNCTION_EMIT_VOID       0x02
#define SYSCALL_DRIVER_FUNCTION_EMIT_U8         0x03

#define SYSCALL_USER_FUNCTION_START             0x00
#define SYSCALL_USER_FUNCTION_DEBUG_PRINT       0x01
#define SYSCALL_USER_FUNCTION_DEV_CHAR_WRITE    0x02
#define SYSCALL_USER_FUNCTION_EXIT              0x03
#define SYSCALL_USER_FUNCTION_SLEEPMS           0x05
#define SYSCALL_USER_FUNCTION_DEV_GET_BY_ID     0x08
#define SYSCALL_USER_FUNCTION_DEV_GET_BY_TYPE   0x09
#define SYSCALL_USER_FUNCTION_DEV_ACQUIRE       0x0a
#define SYSCALL_USER_FUNCTION_DEV_RELEASE       0x0b
#define SYSCALL_USER_FUNCTION_DEV_MEM_WRITE     0x0c
#define SYSCALL_USER_FUNCTION_FUG               0x0d
#define SYSCALL_USER_FUNCTION_DEV_LISTEN        0x0e

/*
    Important note:

    When setting syscall parameters
    remember that
    SYSCALL_PAR* must be called before SYSCALL_FUNC
    as EAX register is used as intermediate register
    and stores the last assigned value
*/
#define SYSCALL_DECL static inline
#define SYSCALL_FUNC(LEVEL, CODE) REG(32, _code, eax); _code = (uint_32) SYSCALL_ ## LEVEL ## _FUNCTION_ ## CODE ;
#define SYSCALL_DRIVER_FUNC(CODE) SYSCALL_FUNC(DRIVER, CODE)
#define SYSCALL_USER_FUNC(CODE) SYSCALL_FUNC(USER, CODE)
#define SYSCALL_INT(CODE) { __asm__ volatile ("int $" STR(CODE) ); }
#define SYSCALL_DRIVER_INT SYSCALL_INT(SYSCALL_DRIVER_INTERRUPT)
#define SYSCALL_USER_INT SYSCALL_INT(SYSCALL_USER_INTERRUPT)
#define REG_RET(BITS, NAME) REG(BITS, NAME, eax)
#define SYSCALL_RETVAL(BITS) { REG_RET(BITS, _retVal); return _retVal; }

#define SYSCALL_PAR(NAME, VALUE, REG_NAME) REG(32, _param ## NAME , REG_NAME); _param ## NAME = (uint_32) VALUE ;
#define SYSCALL_PAR1(VALUE) SYSCALL_PAR(1, VALUE, ebx)
#define SYSCALL_PAR2(VALUE) SYSCALL_PAR(2, VALUE, ecx)

#endif // GUNWOS_SCL_DEF_H
