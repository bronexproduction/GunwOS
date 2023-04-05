//
//  gunwdevemitter.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWDEVEMITTER_H
#define GUNWOS_GUNWDEVEMITTER_H

#include <types.h>
#include <scl_def.h>
#include <gunwdev.h>

/*
    Emit device event without associated data

    Params:
        * type - event type (driver specific)

    Note:
        * Not allowed from user-level
*/
SYSCALL_DECL enum gnwDeviceError emit_void(const int_32 type) {
    SYSCALL_PAR1(type);

    SYSCALL_DRIVER_FUNC(EMIT_VOID);
    SYSCALL_DRIVER_INT;

    register enum gnwDeviceError ret __asm__ ("eax");
    return ret;
}

/*
    Emit device event with uint_8 data

    Params:
        * type - event type (driver specific)
        * data - event data

    Note:
        * Not allowed from user-level
*/
SYSCALL_DECL enum gnwDeviceError emit_u8(const int_32 type, const uint_8 data) {
    SYSCALL_PAR1(type);
    SYSCALL_PAR2(data);

    SYSCALL_DRIVER_FUNC(EMIT_U8);
    SYSCALL_DRIVER_INT;
    
    register enum gnwDeviceError ret __asm__ ("eax");
    return ret;
}

#endif // GUNWOS_GUNWDEVEMITTER_H
