//
//  io.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.03.2020.
//

#include <stdgunw/types.h>

/*
    I/O wait routine

    NOTE: Use ONLY before system timer becomes available

    Source: https://wiki.osdev.org/Inline_Assembly/Examples

    TODO: Find other (better) way to do it
*/
__attribute__((always_inline)) static inline void k_io_wait(void) {
    __asm__ volatile ( "jmp 1f\n\t"
                       "1:jmp 2f\n\t"
                       "2:" );
}