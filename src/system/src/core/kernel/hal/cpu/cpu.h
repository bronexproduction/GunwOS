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
    uint_16 ds;
    uint_16 es;
    uint_16 fs;
    uint_16 gs;

    /*
        Status register
    */
    uint_32 eflags;
};

/*
    Task State Segment
*/
#warning TO BE DOCUMENTED and checked if the values are aligned correctly
struct __attribute__((packed, aligned(4))) k_cpu_tss {
    uint_16 prevTss;
    uint_32 esp0;
    uint_16 ss0;
    uint_32 esp1;
    uint_16 ss1;
    uint_32 esp2;
    uint_16 ss2;
    uint_32 cr3;
    uint_32 eip;
    uint_32 eflags;
    uint_32 eax;
    uint_32 ecx;
    uint_32 edx;
    uint_32 ebx;
    uint_32 esp;
    uint_32 ebp;
    uint_32 esi;
    uint_32 edi;
    uint_16 es;
    uint_16 cs;
    uint_16 ss;
    uint_16 ds;
    uint_16 fs;
    uint_16 gs;
    uint_16 ldt;
    struct __attribute__((packed, aligned(2))) {
        bool trap;
        uint_16 ioMapBase;
    };
};

extern struct k_cpu_tss k_cpu_tss;

#endif // CPU_H
