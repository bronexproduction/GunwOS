//
//  syscall.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <stdgunw/types.h>
#include <stdgunw/mem.h>
#include "../../log/log.h"

#include "func.h"

#define SYSCALL_COUNT 12

/*
    Array of pointers to syscall handlers

    Array index corresponds to syscall function code
*/
static void (*syscallReg[SYSCALL_COUNT])() = {
    /* 0x00 */ 0,
    /* 0x01 */ k_scr_rdb,
    /* 0x02 */ k_scr_wrb,
    /* 0x03 */ k_scr_exit,
    /* 0x04 */ k_scr_dispatch,
    /* 0x05 */ k_scr_sleepms,
    /* 0x06 */ (void *)k_scr_devInstall,
    /* 0x07 */ (void *)k_scr_devStart,
    /* 0x08 */ (void *)k_scr_devGetById,
    /* 0x09 */ (void *)k_scr_devGetByType,
    /* 0x0a */ (void *)k_scr_devAcquire,
    /* 0x0b */ k_scr_fug
};

/*
    Syscall interrupt (int 0x69) request global service routine

    NOTE: Function number has to be put in EAX register
    before making jump to k_scl_syscall label
*/
__attribute__((naked)) void k_scl_syscall() {
    /*
        Syscall function number
    */
    register uint_32 function __asm__ ("eax");

    __asm__ volatile ("pushl %ebx");
    __asm__ volatile ("cmp %%ebx, %%eax" : : "b" (SYSCALL_COUNT));
    __asm__ volatile ("popl %ebx");
    __asm__ volatile ("jae k_scl_syscall_functionOverLimitFailure");

    // /*
    //     Checking if requested syscall function is available
    // */
    register void (*scr)() __asm__ ("eax") = syscallReg[function];
    if (!scr) {
        __asm__ volatile ("jmp k_scl_syscall_serviceRoutineUnavailable");
    }

    /*
        Calling requested syscall function
    */
    __asm__ volatile ("call *%0" : : "r" (scr));
    
    /*
        Finished servicing

        Enables interrupts and returns from interrupt

        NOTE: service routines MUST end with ret
    */

    __asm__ volatile ("k_scl_syscall_end:");

    /*
        Replace caller EAX value on the stack with current value
        
        It puts the result into the EAX register or just does nothing
    */

    __asm__ volatile ("movl %eax, 32(%esp)");
    __asm__ volatile ("ret");

    // /*
    //     Handling syscall service routine unavailable 
    // */
    __asm__ volatile ("k_scl_syscall_serviceRoutineUnavailable:");
    LOG_DEBUG("Syscall function code unavailable");
    __asm__ volatile ("jmp k_scl_syscall_end");

    /*
        Handling error - Syscall function number over limit
    */
    __asm__ volatile ("k_scl_syscall_functionOverLimitFailure:");
    LOG_FATAL("Requested syscall function code over limit");
    __asm__ volatile ("jmp k_scl_syscall_end");
}
