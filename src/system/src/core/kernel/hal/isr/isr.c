//
//  isr.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include "../../../log/log.h"
#include "../../error/panic.h"
#include <stdgunw/types.h>

/*
    Interrupt service routine handling preparation

    Operations:
        - Disable interrupts
        - Push current DS, ES, FS, GS on the stack
        - Push all-purpose registers (just in case)

    Note: Syscall interrupts do not disable maskable interrupts
*/
#define ISR_PUSH { \
    __asm__ volatile ("pushw %ds"); \
    __asm__ volatile ("pushw %es"); \
    __asm__ volatile ("pushw %fs"); \
    __asm__ volatile ("pushw %gs"); \
    __asm__ volatile ("pushal"); \
}

#define ISR_BEGIN   { \
    __asm__ volatile ("cli"); \
    ISR_PUSH \
}

/*
    Interrupt service routine handling end

    Operations:
        - Restore all-purpose registers
        - Restore GS, FS, ES, DS from the stack
        - Switch to the kernel process if needed (after return from interrupt)
        - Enable interrupts
        - Return from interrupt

    Note: Syscall interrupts do not enable maskable interrupts at the end
*/
#define ISR_POP { \
    __asm__ volatile ("popal"); \
    __asm__ volatile ("popw %gs"); \
    __asm__ volatile ("popw %fs"); \
    __asm__ volatile ("popw %es"); \
    __asm__ volatile ("popw %ds"); \
}

#define ISR_END { \
    ISR_POP \
    __asm__ volatile ("sti"); \
    __asm__ volatile ("iret"); \
}

#define ISR_HW(NUM) __attribute__((naked)) void k_isr_picIRQ ## NUM () { \
    ISR_BEGIN \
    __asm__ volatile ("mov $" STR(NUM) ", %eax"); \
    __asm__ volatile ("call k_hal_irqHandle"); \
    extern ptr_t k_que_currentDispatchEntry; \
    if (k_que_currentDispatchEntry) __asm__ volatile ("call k_proc_schedule_intNeedsKernelHandling"); \
    ISR_END \
}

/* 0 */ __attribute__((naked)) void k_isr_divErr() {    OOPS("Division by zero interrupt triggered") }
/* 1 */ __attribute__((naked)) void k_isr_dbgExc() {    LOG_FATAL("Debug exceptions interrupt triggered") }
/* 2 */ __attribute__((naked)) void k_isr_nmi() {       LOG_FATAL("Non-maskable interrupt triggered") }
/* 3 */ __attribute__((naked)) void k_isr_brkpt() {     LOG_FATAL("Breakpoint interrupt triggered") }
/* 4 */ __attribute__((naked)) void k_isr_ofl() {       LOG_FATAL("Overflow interrupt triggered") }
/* 5 */ __attribute__((naked)) void k_isr_bdsChk() {    LOG_FATAL("Bounds check interrupt triggered") }
/* 6 */ __attribute__((naked)) void k_isr_invOpc() {    LOG_FATAL("Invalid opcode interrupt triggered") }
/* 7 */ __attribute__((naked)) void k_isr_coProcNA() {  LOG_FATAL("Coprocessor not available interrupt triggered") }
/* 8 */ __attribute__((naked)) void k_isr_dblFlt() {    LOG_FATAL("Double fault interrupt triggered") }
// 9 (reserved)
/* 10 */ __attribute__((naked)) void k_isr_invTSS() {   LOG_FATAL("Invalid TSS interrupt triggered") }
/* 11 */ __attribute__((naked)) void k_isr_segNP() {    LOG_FATAL("Segment not present interrupt triggered") }
/* 12 */ __attribute__((naked)) void k_isr_stExc() {    LOG_FATAL("Stack exception interrupt triggered") }
/* 13 */ __attribute__((naked)) void k_isr_genPrt() {   OOPS("General protection interrupt triggered") }
/* 14 */ __attribute__((naked)) void k_isr_pgFlt() {    LOG_FATAL("Page fault interrupt triggered") }
// 15 (reserved)
/* 16 */ __attribute__((naked)) void k_isr_coProcErr() { LOG_FATAL("Coprocessor error interrupt triggered") }
// 17 - 31 (reserved)

/*
    32-255 Available for external interrupts via INTR pin
*/

/* 32 */ ISR_HW(0)
/* 33 */ ISR_HW(1)
// 34
// 35
// 36
// 37
/* 38 */ ISR_HW(6)
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

/* 69 */ __attribute__((naked)) void k_isr_driverSyscall() {
    ISR_PUSH
    __asm__ volatile ("call k_scl_driverSyscall");
    /* EAX on stack should contain return value (if any) */
    ISR_POP
    __asm__ volatile ("iret");
}

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

/* 105 */ __attribute__((naked)) void k_isr_userSyscall() {
    ISR_PUSH
    __asm__ volatile ("call k_scl_userSyscall");
    /* EAX on stack should contain return value (if any) */
    ISR_POP
    __asm__ volatile ("iret");
}

// 106 - 255