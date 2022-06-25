//
//  gunwio.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.02.2021.
//

#ifndef GUNWOS_GUNWIO_H
#define GUNWOS_GUNWIO_H

#include "scl_def.h"

/*
    Read byte from system bus

    TODO: Restriction to drivers
*/
static inline uint_8 rdb(uint_16 const port) {
    SYSCALL_PAR1(port);

    SYSCALL_FUNC(RDB);
    SYSCALL_INT;
    
    register uint_8 ret __asm__ ("al");
    return ret;
}

/*
    Write byte to system bus

    TODO: Restriction to drivers
*/
static inline void wrb(uint_16 const port, uint_8 const val) {
    SYSCALL_PAR1(port);
    SYSCALL_PAR2(val);

    SYSCALL_FUNC(WRB);
    SYSCALL_INT;
}

/*
    Prints character array

    Params:
        * str - pointer to the char array
        * len - length of char string to be printed
         
        Note: If length set to 0 the string is printed until '\0' character is reached
*/
static inline uint_32 printl(const char * const str, uint_32 const len) {
    SYSCALL_PAR1(str);
    SYSCALL_PAR2(len); 

    SYSCALL_FUNC(PRINTL);
    SYSCALL_INT;
    
    register uint_32 ret __asm__ ("eax");
    return ret;
}

/*
    Prints character array

    Params:
        * str - pointer to the char array
         
        Note: The string is printed until '\0' character is reached
*/
static inline uint_32 print(const char * const str) {
    return printl(str, 0);
}

#endif // GUNWOS_GUNWIO_H
