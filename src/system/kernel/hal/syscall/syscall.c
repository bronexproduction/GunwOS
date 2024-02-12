//
//  syscall.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <types.h>
#include <mem.h>
#include <error/panic.h>

#include "drvfunc.h"
#include "usrfunc.h"
#include "stack.h"

#define DRIVER_SYSCALL_COUNT 3
#define USER_SYSCALL_COUNT 21

/*
    Array of pointers to driver syscall handlers

    Array index corresponds to syscall function code
*/
static void (*syscallReg_DRIVER[DRIVER_SYSCALL_COUNT])() = {
    /* 0x00 */ (void *)k_scr_rdb,
    /* 0x01 */ (void *)k_scr_wrb,
    /* 0x02 */ (void *)k_scr_emit
};

/*
    Array of pointers to user-level syscall handlers

    Array index corresponds to syscall function code
*/
static void (*syscallReg_USER[USER_SYSCALL_COUNT])() = {
    /* 0x00 */ (void *)k_scr_start,
    /* 0x01 */ (void *)k_scr_log,
    /* 0x02 */ (void *)k_scr_devCharWrite,
    /* 0x03 */ (void *)k_scr_bye,
    /* 0x04 */ (void *)k_scr_waitForEvent,
    /* 0x05 */ (void *)k_scr_timeMs,
    /* 0x06 */ (void *)k_scr_ipcSend,
    /* 0x07 */ (void *)k_scr_ipcRegister,
    /* 0x08 */ (void *)k_scr_devGetById,
    /* 0x09 */ (void *)k_scr_devGetByType,
    /* 0x0a */ (void *)k_scr_devAcquire,
    /* 0x0b */ (void *)k_scr_devRelease,
    /* 0x0c */ (void *)k_scr_devMemWrite,
    /* 0x0d */ (void *)k_scr_fug,
    /* 0x0e */ (void *)k_scr_devListen,
    /* 0x0f */ (void *)k_scr_devGetParam,
    /* 0x10 */ (void *)k_scr_devSetParam,
    /* 0x11 */ (void *)k_scr_runLoopGetItem,
    /* 0x12 */ (void *)k_scr_runLoopGetData,
    /* 0x13 */ (void *)k_scr_ipcReply,
    /* 0x14 */ (void *)k_scr_yield,
};

__attribute__((naked, unused)) static void k_scl_syscall() {
    register void (*scr)() __asm__ ("eax");
    if (!scr) {
        __asm__ volatile ("jmp k_scl_syscall_serviceRoutineUnavailable");
    }

    /*
        Calling requested syscall function
    */
    __asm__ volatile ("call *%0" : : "r" (scr));
    __asm__ volatile ("jmp k_scl_syscall_end");
}

/*
    Handling error - Syscall function number over limit
*/
__attribute__((naked, unused)) static void k_scl_syscall_functionOverLimitFailure() {
    OOPS_NBR("Requested syscall function code over limit");
    __asm__ volatile ("jmp k_scl_syscall_end");
}

/*
    Handling error - Syscall service routine unavailable 
*/
__attribute__((naked, unused)) static void k_scl_syscall_serviceRoutineUnavailable() {
    OOPS_NBR("Syscall function code unavailable");
    __asm__ volatile ("jmp k_scl_syscall_end");
}

/*
    Finish servicing

    Enables interrupts and returns from interrupt

    NOTE: service routines MUST end with ret
*/
__attribute__((naked, unused)) static void k_scl_syscall_end() {
    /*
        Replace caller EAX value on the stack with current value
        
        It puts the result into the EAX register or just does nothing
    */

    __asm__ volatile ("movl %eax, 32(%esp)");
    __asm__ volatile ("ret");
}

#define _SYSCALL_ENTRY(TYPE) __attribute__((naked)) void k_scl_syscall_ ## TYPE () {                    \
    uint_32 * function = FUNC_CODE_PTR;                                                                     \
    if (*function >= TYPE ## _SYSCALL_COUNT) {                                                          \
        __asm__ volatile ("jmp k_scl_syscall_functionOverLimitFailure");                                \
    }                                                                                                   \
    register void (*scr)() __asm__ ("eax") __attribute__((unused)) = syscallReg_ ## TYPE [*function];   \
    __asm__ volatile ("jmp k_scl_syscall");                                                             \
}

/*
    Driver syscall interrupt (int 0x68) request global service routine

    NOTE: Function number has to be put in EAX register
    before making jump to k_scl_driverSyscall label
*/
_SYSCALL_ENTRY(DRIVER);

/*
    User-level syscall interrupt (int 0x69) request global service routine

    NOTE: Function number has to be put in EAX register
    before making jump to k_scl_userSyscall label
*/
_SYSCALL_ENTRY(USER);
