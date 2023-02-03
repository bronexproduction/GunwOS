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
#include <stdgunw/utils.h>
#include <gunwdrv.h>

#define SCR_END {__asm__ volatile ("ret");};
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; SCR_END }

/*
    Driver-level system calls
*/

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
    Code - 0x06
    Function - DEV_INSTALL

    Params:
        * EBX - device identifier return pointer
        * ECX - device descriptor (struct gnwDeviceDescriptor *)

    Return:
        * EAX - error code (enum gnwDriverError)
*/

SCR(devInstall,
    REG(32, id, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const);
    err = k_dev_install((size_t * const)id, (struct gnwDeviceDescriptor*)desc);
)

/*
    Code - 0x07
    Function - DEV_START

    Params:
        * EBX - device identifier

    Return:
        * EAX - error code (enum gnwDriverError)
*/
SCR(devStart,
    REG(32, id, ebx)

    REG_RET(32, err)

    enum gnwDriverError k_dev_start(size_t id);
    err = k_dev_start((size_t)id);
)
