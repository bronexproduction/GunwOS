//
//  isr.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <types.h>

#include <error/panic.h>
#include <hal/cpu/cpu.h>

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
    CPU_SEG_RESTORE \
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
    if (!isrStackHeight && k_que_currentDispatchEntry) { \
        __asm__ volatile ("pushl %esp"); \
        __asm__ volatile ("call k_proc_schedule_intNeedsKernelHandling"); \
        __asm__ volatile ("addl $4, %esp"); \
    } \
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

/* 0 */ __attribute__((naked)) void k_isr_divErr() {        CPU_SEG_RESTORE; OOPS("Division by zero interrupt triggered"); }
/* 1 */ __attribute__((naked)) void k_isr_dbgExc() {        CPU_SEG_RESTORE; OOPS("Debug exceptions interrupt triggered"); }
/* 2 */ __attribute__((naked)) void k_isr_nmi() {           CPU_SEG_RESTORE; OOPS("Non-maskable interrupt triggered"); }
/* 3 */ __attribute__((naked)) void k_isr_brkpt() {         CPU_SEG_RESTORE; OOPS("Breakpoint interrupt triggered"); }
/* 4 */ __attribute__((naked)) void k_isr_ofl() {           CPU_SEG_RESTORE; OOPS("Overflow interrupt triggered"); }
/* 5 */ __attribute__((naked)) void k_isr_bdsChk() {        CPU_SEG_RESTORE; OOPS("Bounds check interrupt triggered"); }
/* 6 */ __attribute__((naked)) void k_isr_invOpc() {        CPU_SEG_RESTORE; OOPS("Invalid opcode interrupt triggered"); }
/* 7 */ __attribute__((naked)) void k_isr_coProcNA() {      CPU_SEG_RESTORE; OOPS("Coprocessor not available interrupt triggered"); }
/* 8 */ __attribute__((naked)) void k_isr_dblFlt() {        CPU_SEG_RESTORE; OOPS("Double fault interrupt triggered"); }
// 9 (reserved)
/* 10 */ __attribute__((naked)) void k_isr_invTSS() {       CPU_SEG_RESTORE; OOPS("Invalid TSS interrupt triggered"); }
/* 11 */ __attribute__((naked)) void k_isr_segNP() {        CPU_SEG_RESTORE; OOPS("Segment not present interrupt triggered"); }
/* 12 */ __attribute__((naked)) void k_isr_stExc() {        CPU_SEG_RESTORE; OOPS("Stack exception interrupt triggered"); }
/* 13 */ __attribute__((naked)) void k_isr_genPrt() {       CPU_SEG_RESTORE; OOPS_WITH_CODE("General protection interrupt triggered"); }
/* 14 */ __attribute__((naked)) void k_isr_pgFlt() {        CPU_SEG_RESTORE; OOPS("Page fault interrupt triggered"); }
// 15 (reserved)
/* 16 */ __attribute__((naked)) void k_isr_coProcErr() {    CPU_SEG_RESTORE; OOPS("Coprocessor error interrupt triggered"); }
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