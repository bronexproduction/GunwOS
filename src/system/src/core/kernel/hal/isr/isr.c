//
//  isr.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <stdgunw/types.h>

#include "../../../log/log.h"
#include "../../error/panic.h"
#include "../cpu/cpu.h"

/*
    ISR stack height counter

    Counts how many interrupt service routines 
    are currently on the stack
*/
static size_t isrStackHeight = 0;

/*
    Interrupt service routine handling preparation

    Operations:
        - Disable interrupts
        - Save CPU state on stack
        - Restore DS, ES, FS, GS values according to loaded SS
        - Increment ISR stack height counter

    Note: Syscall interrupts do not disable maskable interrupts - to be implemented
*/
#warning TO BE IMPLEMENTED - up
#define ISR_BEGIN   { \
    __asm__ volatile ("cli"); \
    CPU_PUSH \
    __asm__ volatile ("pushl %eax"); \
    __asm__ volatile ("movw %ss, %ax"); \
    __asm__ volatile ("movw %ax, %ds"); \
    __asm__ volatile ("movw %ax, %es"); \
    __asm__ volatile ("movw %ax, %fs"); \
    __asm__ volatile ("movw %ax, %gs"); \
    __asm__ volatile ("popl %eax"); \
    __asm__ volatile ("incl %[mem]" : [mem] "=m" (isrStackHeight)); \
}

/*
    Interrupt service routine handling end

    Operations:
        - Decrement ISR stack height counter
        - Request kernel handling if current interrupt 
          is the last on the stack and the queue is not empty
        - Restore CPU state
        - Enable interrupts
        - Return from interrupt

    Note: Syscall interrupts do not enable maskable interrupts at the end - to be implemented
*/
#warning TO BE IMPLEMENTED - up
#define ISR_END { \
    __asm__ volatile ("decl %[mem]" : [mem] "=m" (isrStackHeight)); \
    extern ptr_t k_que_currentDispatchEntry; \
    if (!isrStackHeight && k_que_currentDispatchEntry) __asm__ volatile ("call k_proc_schedule_intNeedsKernelHandling"); \
    CPU_POP \
    __asm__ volatile ("sti"); \
    __asm__ volatile ("iret"); \
}

/*
    Hardware interrupt service routine handler

    Operations:
        - Disable interrupts
        - Save CPU state
        - Call IRQ handler
        - Switch to the kernel process if needed (after return from interrupt)
        - Restore CPU state
        - Enable interrupts
        - Return from interrupt
*/
#define ISR_HW(NUM) __attribute__((naked)) void k_isr_picIRQ ## NUM () { \
    ISR_BEGIN \
    __asm__ volatile ("mov $" STR(NUM) ", %eax"); \
    __asm__ volatile ("call k_hal_irqHandle"); \
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
/* 13 */ __attribute__((naked)) void k_isr_genPrt() {   OOPS_WITH_CODE("General protection interrupt triggered") }
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
    ISR_BEGIN
    #warning SYSTEM CALLS CAN CAUSE KERNEL LOCKS - to be analysed
    __asm__ volatile ("call k_scl_driverSyscall");
    /* EAX stored for current process should contain return value (if any) */
    ISR_END
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
    ISR_BEGIN
    #warning SYSTEM CALLS CAN CAUSE KERNEL LOCKS - to be analysed
    __asm__ volatile ("call k_scl_userSyscall");
    /* EAX stored for current process should contain return value (if any) */
    ISR_END
}

// 106 - 255