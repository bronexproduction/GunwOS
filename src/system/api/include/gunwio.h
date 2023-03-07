//
//  gunwio.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef GUNWOS_GUNWIO_H
#define GUNWOS_GUNWIO_H

#include "scl_def.h"
#include <gunwdev.h>

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

/*
    Write character to character output device

    Parameters:
    * character - Character to be written
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError charOutWrite(uint_32 deviceId, const char character) {
    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(character);

    SYSCALL_USER_FUNC(CHAR_OUT_WRITE);
    SYSCALL_USER_INT;
    
    register enum gnwDeviceError ret __asm__ ("eax");
    return ret;
}

#endif // GUNWOS_GUNWIO_H
