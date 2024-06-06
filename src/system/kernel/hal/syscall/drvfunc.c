//
//  drvfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <hal/io/bus.h>
#include "func.h"
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
        * RESULT_PTR - value read from bus port
*/
void k_scr_rdb(const ptr_t refEsp) {
    PAR_PTR_1(port);

    *RESULT_PTR = k_bus_inb(*port);
}

/*
    Code - 0x01
    Function - WRB

    Params:
        * PAR_PTR_1 - port
        * PAR_PTR_2 - value

*/
void k_scr_wrb(const ptr_t refEsp) {
    PAR_PTR_1(port);
    PAR_PTR_2(value);

    k_bus_outb(*port, *value);
}

/*
    Code - 0x02
    Function - EMIT

    Params:
        * PAR_PTR_1 - struct gnwDeviceEvent pointer relative to caller process memory
                      (to be implemented when drivers moved to process space)
        
    Return:
        * RESULT_PTR - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
void k_scr_emit(const ptr_t refEsp) {
    PAR_PTR_1(eventPtr);

    *RESULT_PTR = k_dev_emit((struct gnwDeviceEvent *)*eventPtr);
}
