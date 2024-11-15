//
//  isr.c
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2020.
//

#include <types.h>

#include <error/panic.h>
#include <hal/cpu/cpu.h>
#include <hal/hal.h>
#include <hal/syscall/syscall.h>
#include <schedule/schedule.h>

/*
    ISR stack height counter

    Counts how many interrupt service routines 
    are currently on the stack
*/
size_t isrStackHeight = 0;

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
#define _ISR_BEGIN_PROLOGUE \
    _CPU_INTERRUPTS_DISABLE "\n" \
    _CPU_PUSH "\n" \
    _CPU_SEG_RESTORE   
#define ISR_BEGIN { \
    ++isrStackHeight; \
}
#define _ISR(NAME, HANDLING_CODE) \
    __asm__ ( \
        ".global " STR(NAME) "\n" \
        STR(NAME) ":" "\n" \
        _ISR_BEGIN_PROLOGUE "\n" \
    ); \
    static __attribute__((naked, unused)) void NAME ## _handler() { \
        ISR_BEGIN; \
        { HANDLING_CODE; } \
        ISR_END; \
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
    --isrStackHeight; \
    extern ptr_t k_que_currentDispatchEntry; \
    if (!isrStackHeight && k_que_currentDispatchEntry) { \
        k_proc_schedule_intNeedsKernelHandling((uint_32)k_cpu_stackPtr); \
    } \
    CPU_POP \
    CPU_INTERRUPTS_ENABLE; \
    CPU_INTERRUPT_RETURN; \
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
#define ISR_HW(NUM) _ISR(k_isr_picIRQ ## NUM, { \
    k_hal_irqHandle(NUM); \
})

#warning SYSTEM CALLS CAN CAUSE KERNEL LOCKS - to be analysed
#define ISR_SYSCALL(TYPE) _ISR(k_isr_syscall ## TYPE, { \
    k_scl_syscall_ ## TYPE (k_cpu_stackPtr); \
    /* EAX stored for current process should contain return value (if any) */ \
})

/* 0 */ __attribute__((naked)) void k_isr_divErr() {        CPU_SEG_RESTORE; OOPS_NBR("Division by zero interrupt triggered"); k_cpu_halt(); }
/* 1 */ __attribute__((naked)) void k_isr_dbgExc() {        CPU_SEG_RESTORE; OOPS_NBR("Debug exceptions interrupt triggered"); k_cpu_halt(); }
/* 2 */ __attribute__((naked)) void k_isr_nmi() {           CPU_SEG_RESTORE; OOPS_NBR("Non-maskable interrupt triggered"); k_cpu_halt(); }
/* 3 */ __attribute__((naked)) void k_isr_brkpt() {         CPU_SEG_RESTORE; OOPS_NBR("Breakpoint interrupt triggered"); k_cpu_halt(); }
/* 4 */ __attribute__((naked)) void k_isr_ofl() {           CPU_SEG_RESTORE; OOPS_NBR("Overflow interrupt triggered"); k_cpu_halt(); }
/* 5 */ __attribute__((naked)) void k_isr_bdsChk() {        CPU_SEG_RESTORE; OOPS_NBR("Bounds check interrupt triggered"); k_cpu_halt(); }
/* 6 */ __attribute__((naked)) void k_isr_invOpc() {        CPU_SEG_RESTORE; OOPS_NBR("Invalid opcode interrupt triggered"); k_cpu_halt(); }
/* 7 */ __attribute__((naked)) void k_isr_coProcNA() {      CPU_SEG_RESTORE; OOPS_NBR("Coprocessor not available interrupt triggered"); k_cpu_halt(); }
/* 8 */ __attribute__((naked)) void k_isr_dblFlt() {        CPU_SEG_RESTORE; OOPS_NBR("Double fault interrupt triggered"); k_cpu_halt(); }
// 9 (reserved)
/* 10 */ __attribute__((naked)) void k_isr_invTSS() {       CPU_SEG_RESTORE; OOPS_NBR("Invalid TSS interrupt triggered"); k_cpu_halt(); }
/* 11 */ __attribute__((naked)) void k_isr_segNP() {        CPU_SEG_RESTORE; OOPS_NBR("Segment not present interrupt triggered"); k_cpu_halt(); }
/* 12 */ __attribute__((naked)) void k_isr_stExc() {        CPU_SEG_RESTORE; OOPS_NBR("Stack exception interrupt triggered"); k_cpu_halt(); }
/* 13 */ __attribute__((naked)) void k_isr_genPrt() {       CPU_SEG_RESTORE; OOPS_WITH_CODE_NBR("General protection interrupt triggered"); k_cpu_halt(); }
/* 14 */ __attribute__((naked)) void k_isr_pgFlt() {        CPU_SEG_RESTORE; OOPS_NBR("Page fault interrupt triggered"); k_cpu_halt(); }
// 15 (reserved)
/* 16 */ __attribute__((naked)) void k_isr_coProcErr() {    CPU_SEG_RESTORE; OOPS_NBR("Coprocessor error interrupt triggered"); k_cpu_halt(); }
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
/* 69 */ ISR_SYSCALL(DRIVER)
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
/* 105 */ ISR_SYSCALL(USER)
// 106 - 255