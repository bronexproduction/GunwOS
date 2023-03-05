//
//  drvfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/

#include "../hal/io/bus.h"
#include "func.h"

/*
    Driver-level system calls
*/

/*
    Code - 0x00
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
    Code - 0x01
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
