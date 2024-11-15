//
//  isr.h
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//
//  Interrupt codes and descriptions taken from
//  INTEL 80386 PROGRAMMER'S REFERENCE MANUAL (1986)
//  https://css.csail.mit.edu/6.858/2014/readings/i386.pdf
//

/*
    CPU Exceptions
*/

__attribute__((naked)) void k_isr_divErr();     // 0   - Divide error
__attribute__((naked)) void k_isr_dbgExc();     // 1   - Debug exceptions
__attribute__((naked)) void k_isr_nmi();        // 2   - Nonmaskable interrupt
__attribute__((naked)) void k_isr_brkpt();      // 3   - Breakpoint (one-byte INT 3 instruction)
__attribute__((naked)) void k_isr_ofl();        // 4   - Overflow (INTO instruction)
__attribute__((naked)) void k_isr_bdsChk();     // 5   - Bounds check (BOUND instruction)
__attribute__((naked)) void k_isr_invOpc();     // 6   - Invalid opcode
__attribute__((naked)) void k_isr_coProcNA();   // 7   - Coprocessor not available
__attribute__((naked)) void k_isr_dblFlt();     // 8   - Double fault
// 9 (reserved)
__attribute__((naked)) void k_isr_invTSS();     // 10  - Invalid TSS
__attribute__((naked)) void k_isr_segNP();      // 11  - Segment not present
__attribute__((naked)) void k_isr_stExc();      // 12  - Stack exception
__attribute__((naked)) void k_isr_genPrt();     // 13  - General protection
__attribute__((naked)) void k_isr_pgFlt();      // 14  - Page fault
// 15 (reserved)
__attribute__((naked)) void k_isr_coProcErr();  // 16  - Coprecessor error
// 17-31 (reserved)

/*
    External interrupts

    (32 - 255 Available for external interrupts via INTR pin)
*/

__attribute__((naked)) void k_isr_picIRQ0();    // 32   - IRQ 0 PIC input (remapped)
__attribute__((naked)) void k_isr_picIRQ1();    // 33   - IRQ 1 PIC input (remapped)
// 34
// 35
// 36
// 37
__attribute__((naked)) void k_isr_picIRQ6();    // 38   - IRQ 6 PIC input (remapped)
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
__attribute__((naked)) void k_isr_syscallDRIVER();  // 69   - Driver system call
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
__attribute__((naked)) void k_isr_syscallUSER();    // 105   - User system call
// 106 - 255
