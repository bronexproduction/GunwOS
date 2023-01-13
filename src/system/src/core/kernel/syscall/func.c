//
//  func.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/

#include "../hal/io/bus.h"
#include <stdgunw/utils.h>
#include <gunwdispatch.h>

#define SCR_END {__asm__ volatile ("ret");};
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; SCR_END }

/*
    Code - 0x01
    Function - RDB

    Params:
        * BX - port
        
    Return:
        * AL - value read from bus port
*/
SCR(rdb,
    REG(16, port, bx)
    REG(8, value, al)

    value = k_bus_inb(port);
)

/*
    Code - 0x02
    Function - WRB

    Params:
    
        * BX - port
        * CL - value

*/
SCR(wrb,
    REG(16, port, bx)
    REG(8, value, cl)

    k_bus_outb(port, value);
)

/*
    Code - 0x03
    Function - EXIT

    Params:
    
        * EBX - status

*/
// TODO: wip
SCR(exit,
    REG(32, status, ebx)
)

/*
    Code - 0x04
    Function - PRINTL

    Params:
    
        * EBX - pointer to char array
        * ECX - string length

*/
#include "../../terminal/terminal.h"
SCR(printl,
    REG(32, str, ebx)
    REG(32, len, ecx)

    REG(32, wrCount, eax)

    __asm__ volatile ("cmpl $0, %ecx");
    __asm__ volatile ("jz k_scr_printl_atZeroLength");

    wrCount = k_trm_putsl((char *)str, len);
    __asm__ volatile ("jmp k_scr_printl_end");

    __asm__ volatile ("k_scr_printl_atZeroLength:");
    wrCount = k_trm_puts((char *)str);

    __asm__ volatile ("k_scr_printl_end:")
)

/*
    Code - 0x05
    Function - DISPATCH

    Params:
        * EBX - struct gnwDispatchDesc pointer
*/
SCR(dispatch,
    REG(32, descPtr, ebx)

    extern void k_rlp_dispatch(const struct gnwDispatchDesc * const);
    k_rlp_dispatch((struct gnwDispatchDesc *)descPtr);
)

/*
    Code - 0x06
    Function - SLEEPMS

    Params:
        * EBX - time in milliseconds
*/
SCR(sleepms,
    REG(32, timems, ebx)

    extern void k_tmr_sleepms(const unsigned int);
    k_tmr_sleepms(timems);
)
