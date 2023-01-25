//
//  cpu.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef CPU_H
#define CPU_H

#include <stdgunw/types.h>

struct k_cpu_state {
    /*
        Main registers
    */
    uint_32 eax;
    uint_32 ecx;
    uint_32 edx;
    uint_32 ebx;

    /*
        Index registers
    */
    uint_32 esp;
    uint_32 ebp;
    uint_32 esi;
    uint_32 edi;

    /*
        Instruction pointer
    */
    uint_32 eip;
    
    /*
        Segment selectors
    */
    uint_16 cs;
    uint_16 ds;
    uint_16 es;
    uint_16 fs;
    uint_16 gs;
    uint_16 ss;

    /*
        Status register
    */
    uint_32 eflags;
};

#endif // CPU_H
