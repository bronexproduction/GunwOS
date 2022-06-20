//
//  bus.h
//  GunwOS
//
//  Created by Artur Danielewski on 31.01.2018.
//  Mostly copied & pasted from OS-dev wiki
//

#include "../../../../../lib/stdgunw/types.h"

static inline void k_bus_outb(uint_16 port, uint_8 val) {
    __asm__ volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint_16.  %w1 could be used if we had the port number a wider C type */
}

static inline uint_8 k_bus_inb(uint_16 port) {
    uint_8 ret;
    __asm__ volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
