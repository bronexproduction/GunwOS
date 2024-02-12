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
        * PAR_PTR_1 - port
        
    Return:
        * AL - value read from bus port
*/
SCR(rdb,
    PAR_PTR_1(port)

    REG_RET(8, value)

    value = k_bus_inb(*port);
)

/*
    Code - 0x01
    Function - WRB

    Params:
        * PAR_PTR_1 - port
        * PAR_PTR_2 - value

*/
SCR(wrb,
    PAR_PTR_1(port);
    PAR_PTR_2(value);

    k_bus_outb(*port, *value);
)

/*
    Code - 0x02
    Function - EMIT

    Params:
        * PAR_PTR_1 - struct gnwDeviceEvent pointer relative to caller process memory
                      (to be implemented when drivers moved to process space)
        
    Return:
        * EAX - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
SCR(emit,
    PAR_PTR_1(eventPtr)

    REG_RET(32, err)

    err = k_dev_emit((struct gnwDeviceEvent *)*eventPtr);
)
