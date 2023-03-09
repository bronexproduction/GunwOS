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
    Function - EMIT_VOID

    Params:
        * EBX - driver-specific event type
        
    Return:
        * EAX - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
SCR(emit_void,
    REG(32, type, ebx)

    REG_RET(32, err)

    #warning GET EMITTER ID

    err = k_dev_emit_void(devId, type);
)

/*
    Code - 0x03
    Function - EMIT_U8

    Params:
        * EBX - driver-specific event type
        * CL - event data
        
    Return:
        * EAX - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
SCR(emit_u8,
    REG(32, type, ebx)
    REG(8, data, cl)

    REG_RET(32, err)

    #warning GET EMITTER ID

    err = k_dev_emit_u8(devId, type, data);
)
