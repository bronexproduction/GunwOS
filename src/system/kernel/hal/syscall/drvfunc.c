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
void k_scr_rdb(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const uint_16, port, PARAMETER_1_STACK_OFFSET);

    #warning look at usr syscalls

    SAFE_STACK_RESULT_ARCH_VAL = k_bus_inb(*port);
}

/*
    Code - 0x01
    Function - WRB

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - port
        * PARAMETER_2_STACK_OFFSET - value

*/
void k_scr_wrb(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const uint_16, port, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const uint_8, value, PARAMETER_2_STACK_OFFSET);

    #warning look at usr syscalls

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
void k_scr_emit(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const struct gnwDeviceEvent * const, eventPtr, PARAMETER_1_STACK_OFFSET);

    SAFE_STACK_RESULT_ARCH_VAL = k_dev_emit(procId, *eventPtr);
}

/*
    Code - 0x03
    Function - MMIO_PLZ

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - size of the requested buffer in bytes
        * PARAMETER_2_STACK_OFFSET - physical memory address to be mapped
        
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code if any, otherwise GDE_NONE (see enum gnwDeviceError)
*/
void k_scr_mmioPlz(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, sizeBytes, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const addr_t, physMemStart, PARAMETER_2_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_drv_mmioPlz(const procId_t, const size_t, const addr_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_drv_mmioPlz(procId, *sizeBytes, *physMemStart);
}
