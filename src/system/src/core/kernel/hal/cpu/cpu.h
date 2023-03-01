//
//  cpu.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef CPU_H
#define CPU_H

#include <stdgunw/types.h>

#define CPU_PUSH { \
    __asm__ volatile ("pushw %ds"); \
    __asm__ volatile ("pushw %es"); \
    __asm__ volatile ("pushw %fs"); \
    __asm__ volatile ("pushw %gs"); \
    __asm__ volatile ("pushal"); \
}

#define CPU_POP { \
    __asm__ volatile ("popal"); \
    __asm__ volatile ("popw %gs"); \
    __asm__ volatile ("popw %fs"); \
    __asm__ volatile ("popw %es"); \
    __asm__ volatile ("popw %ds"); \
}

enum k_cpu_eflags {
    FLAGS_CARRY     = 0x0001,
    FLAGS_PARITY    = 0x0004,
    FLAGS_AUXCARRY  = 0x0010,
    FLAGS_ZERO      = 0x0040,
    FLAGS_SIGN      = 0x0080,
    FLAGS_TRAP      = 0x0100,
    FLAGS_INTERRUPT = 0x0200,
    FLAGS_DIRECTION = 0x0400,
    FLAGS_OVERFLOW  = 0x0800,
    FLAGS_IOPL      = 0x3000,
    FLAGS_NESTED    = 0x4000,
    FLAGS_MODE      = 0x8000,
    EFLAGS_RESUME   = 0x00010000,
    EFLAGS_V8086    = 0x00020000
};

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

void k_cpu_init();

/*
    Loads segment selector field of the task register
*/
void k_cpu_loadTaskRegister();

#endif // CPU_H
