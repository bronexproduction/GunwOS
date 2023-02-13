//
//  idt.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <stdgunw/mem.h>

#include "../gdt/gdt.h"

#define IDT_SIZE_DEFAULT 256

enum k_idt_dpl {
    DPL_0 = 0b00,
    DPL_1 = 0b01,
    DPL_2 = 0b10,
    DPL_3 = 0b11
};

struct __attribute__((packed)) k_idt_entry {
    uint_16 offset_l;   // offset 0-15
    uint_16 selector;   // selector (index of GDT/LDT entry)
    uint_8 zeroed;      // not used or zeroed
    uint_8 flags:5;     // gate type flags
    uint_8 dpl:2;       // Descriptor Privilege Level
    uint_8 p:1;         // is present
    uint_16 offset_h;   // offset 16-31
} k_idt_default[IDT_SIZE_DEFAULT];

static inline void k_idt_load(void *base, uint_16 size) {
    struct __attribute__((packed)) k_idt_descriptor {
        uint_16 limit;      // length of IDT
        uint_32 base;       // memory location (pointer to k_idt_default)
    } idtr;

    idtr.base = (uint_32)base;
    idtr.limit = sizeof(struct k_idt_entry) * size - 1;

    __asm__ ( "lidt %0" : : "m"(idtr) );
}

static struct k_idt_entry k_idt_gate_stub(void (* const proc)(void), const enum k_idt_dpl dpl) {
    struct k_idt_entry d;

    d.offset_l = (uint_16)((uint_32)proc & 0xFFFF);
    d.selector = &k_gdt_gdt.r0code;
    d.zeroed = 0;
    d.dpl = dpl;
    d.p = 0b1;
    d.offset_h = (uint_16)((uint_32)proc >> 16);

    return d;
}

// TODO: Not implemented yet
// static struct k_idt_entry k_idt_gateTask(void (* const proc)(void), const enum k_idt_dpl dpl) {
//     struct k_idt_entry d = k_idt_gate_stub(proc, dpl);
//     d.flags = 0b10100;
//     return d;
// }

static struct k_idt_entry k_idt_gateInterrupt(void (* const proc)(void), const enum k_idt_dpl dpl) {
    struct k_idt_entry d = k_idt_gate_stub(proc, dpl);
    d.flags = 0b01110;    
    return d;
}

// TODO: Not implemented yet
// static struct k_idt_entry k_idt_gateTrap(void (* const proc)(void), const enum k_idt_dpl dpl) {
//     struct k_idt_entry d = k_idt_gate_stub(proc, dpl);
//     d.flags = 0b11110;
//     return d;
// }

#include "../isr/isr.h"

void k_idt_loadDefault() {
    memnull(k_idt_default, sizeof(struct k_idt_entry) * IDT_SIZE_DEFAULT);

    /*
        CPU Exceptions
    */
   
    k_idt_default[0] = k_idt_gateInterrupt(k_isr_divErr, DPL_0);
    k_idt_default[1] = k_idt_gateInterrupt(k_isr_dbgExc, DPL_0);
    k_idt_default[2] = k_idt_gateInterrupt(k_isr_nmi, DPL_0);
    k_idt_default[3] = k_idt_gateInterrupt(k_isr_brkpt, DPL_0);
    k_idt_default[4] = k_idt_gateInterrupt(k_isr_ofl, DPL_0);
    k_idt_default[5] = k_idt_gateInterrupt(k_isr_bdsChk, DPL_0);
    k_idt_default[6] = k_idt_gateInterrupt(k_isr_invOpc, DPL_0);
    k_idt_default[7] = k_idt_gateInterrupt(k_isr_coProcNA, DPL_0);
    k_idt_default[8] = k_idt_gateInterrupt(k_isr_dblFlt, DPL_0);
    // 9 (reserved)
    k_idt_default[10] = k_idt_gateInterrupt(k_isr_invTSS, DPL_0);
    k_idt_default[11] = k_idt_gateInterrupt(k_isr_segNP, DPL_0);
    k_idt_default[12] = k_idt_gateInterrupt(k_isr_stExc, DPL_0);
    k_idt_default[13] = k_idt_gateInterrupt(k_isr_genPrt, DPL_0);
    k_idt_default[14] = k_idt_gateInterrupt(k_isr_pgFlt, DPL_0);
    // 15 (reserved)
    k_idt_default[16] = k_idt_gateInterrupt(k_isr_coProcErr, DPL_0);
    // 17-31 (reserved)
    
    /*
        External interrupts

        (32-255 Available for external interrupts via INTR pin)
    */

    k_idt_default[32] = k_idt_gateInterrupt(k_isr_picIRQ0, DPL_0);
    k_idt_default[33] = k_idt_gateInterrupt(k_isr_picIRQ1, DPL_0);
    // 34
    // 35
    // 36
    // 37
    k_idt_default[38] = k_idt_gateInterrupt(k_isr_picIRQ6, DPL_0);
    // 39
    // 40
    // 41
    // 42
    // 43
    // 44
    // 45
    // 46
    // 47
    // 48
    // 49
    // 50
    // 51
    // 52
    // 53
    // 54
    // 55
    // 56
    // 57
    // 58
    // 59
    // 60
    // 61
    // 62
    // 63
    // 64
    // 65
    // 66
    // 67
    // 68
    k_idt_default[69] = k_idt_gateInterrupt(k_isr_driverSyscall, DPL_0);
    // 70
    // 71
    // 72
    // 73
    // 74
    // 75
    // 76
    // 77
    // 78
    // 79
    // 80
    // 81
    // 82
    // 83
    // 84
    // 85
    // 86
    // 87
    // 88
    // 89
    // 90
    // 91
    // 92
    // 93
    // 94
    // 95
    // 96
    // 97
    // 98
    // 99
    // 100
    // 101
    // 102
    // 103
    // 104
    k_idt_default[105] = k_idt_gateInterrupt(k_isr_userSyscall, DPL_0);
    // 106 - 255

    k_idt_load(&k_idt_default, IDT_SIZE_DEFAULT);
}
