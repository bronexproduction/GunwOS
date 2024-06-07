//
//  drvfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <hal/io/bus.h>
#include "params.h"
#include <dev/dev.h>
#include <hal/proc/proc.h>

/*
    Driver-level system calls
*/

/*
    Code - 0x00
    Function - RDB

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - port
        
    Return (process stack offset):
        * RESULT_STACK_OFFSET - value read from bus port
*/
void k_scr_rdb(const ptr_t refEsp) {
    const uint_16 * const port = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_bus_inb(*port);
}

/*
    Code - 0x01
    Function - WRB

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - port
        * PARAMETER_2_STACK_OFFSET - value

*/
void k_scr_wrb(const ptr_t refEsp) {
    const uint_16 * const port = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const uint_8 * const value = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);

    k_bus_outb(*port, *value);
}

/*
    Code - 0x02
    Function - EMIT

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - struct gnwDeviceEvent pointer relative to caller process memory
                                     (to be implemented when drivers moved to process space)
        
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
void k_scr_emit(const ptr_t refEsp) {
    const struct gnwDeviceEvent * const * const eventPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_dev_emit(k_proc_getCurrentId(), *eventPtr);
}
