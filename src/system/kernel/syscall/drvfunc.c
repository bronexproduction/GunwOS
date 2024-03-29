//
//  drvfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <hal/io/bus.h>
#include "func.h"
#include <scl_def.h>
#include <dev/dev.h>

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

    REG_RET(8, value)

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

/*
    Code - 0x02
    Function - EMIT

    Params:
        * EBX - struct gnwDeviceEvent pointer relative to caller process memory
                (to be implemented when drivers moved to process space)
        
    Return:
        * EAX - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
SCR(emit,
    REG(32, eventPtr, ebx)

    REG_RET(32, err)

    err = k_dev_emit((struct gnwDeviceEvent *)eventPtr);
)
