//
//  cpu.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef CPU_H
#define CPU_H

#include <types.h>

#define CPU_INTERRUPTS_DISABLE __asm__ volatile ("cli");
#define CPU_INTERRUPTS_ENABLE __asm__ volatile ("sti");
#define CPU_INTERRUPT_RETURN __asm__ volatile ("iret");
#define CPU_RETURN __asm__ volatile ("ret");

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

#define CPU_SEG_RESTORE { \
    __asm__ volatile ("movw %ss, %ax"); \
    __asm__ volatile ("movw %ax, %ds"); \
    __asm__ volatile ("movw %ax, %es"); \
    __asm__ volatile ("movw %ax, %fs"); \
    __asm__ volatile ("movw %ax, %gs"); \
}

register const ptr_t k_cpu_stackPtr __asm__ ("esp");

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

    See:
    INTEL 80386 PROGRAMMER'S REFERENCE MANUAL 1986
    Figure 7-1. 80386 32-Bit Task State Segment
*/
struct __attribute__((packed)) k_cpu_tss {
    uint_16 prevTss;
    const uint_16 _res0;
    uint_32 esp0;
    uint_16 ss0;
    const uint_16 _res1;
    uint_32 esp1;
    uint_16 ss1;
    const uint_16 _res2;
    uint_32 esp2;
    uint_16 ss2;
    const uint_16 _res3;
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
    const uint_16 _res4;
    uint_16 cs;
    const uint_16 _res5;
    uint_16 ss;
    const uint_16 _res6;
    uint_16 ds;
    const uint_16 _res7;
    uint_16 fs;
    const uint_16 _res8;
    uint_16 gs;
    const uint_16 _res9;
    uint_16 ldt;
    const uint_16 _res10;
    bool trap               :1;
    const uint_16 _res11    :15;
    uint_16 ioMapBase;
};
_Static_assert(sizeof(struct k_cpu_tss) == 104, "Unexpected k_cpu_tss size");

extern struct k_cpu_tss k_cpu_tss;

void k_cpu_init();

/*
    Loads segment selector field of the task register
*/
void k_cpu_loadTaskRegister();

/*
    Halts the CPU
*/
void k_cpu_halt();

#endif // CPU_H
