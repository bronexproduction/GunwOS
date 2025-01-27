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
#include <gunwmemtypes.h>

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

    extern uint_8 k_scr_drv_rdb(const procId_t, const uint_16);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_drv_rdb(procId, *port);
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

    extern void k_scr_drv_wrb(const procId_t, const uint_16, const uint_8);
    k_scr_drv_wrb(procId, *port, *value);
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

    extern enum gnwDeviceError k_scr_drv_emit(const procId_t, const struct gnwDeviceEvent * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_drv_emit(procId, *eventPtr);
}

/*
    Code - 0x03
    Function - MMIO_PLZ

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - number of contiguous physical pages to be mapped
        * PARAMETER_2_STACK_OFFSET - linear (process) memory address to be mapped
        * PARAMETER_3_STACK_OFFSET - physical memory address to be mapped
        
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error (enum gnwMemoryError) if anything goes wrong, otherwise GME_NONE
*/
void k_scr_mmioPlz(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const addr_t, pageCount, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const addr_t, vAddr, PARAMETER_2_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const addr_t, pAddr, PARAMETER_3_STACK_OFFSET);

    extern enum gnwMemoryError k_scr_drv_mmioPlz(const procId_t, const size_t, const addr_t, const addr_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_drv_mmioPlz(procId, *pageCount, *vAddr, *pAddr);
}

/*
    Code - 0x04
    Function - REPORT_INIT

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - driver initialization report status
*/
void k_scr_reportInit(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const bool, success, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_drv_reportInit(const procId_t, const bool);
    k_scr_drv_reportInit(procId, *success);
}

/*
    Code - 0x05
    Function - REPORT_START

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - driver startup report status
*/
void k_scr_reportStart(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const bool, success, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_drv_reportStart(const procId_t, const bool);
    k_scr_drv_reportStart(procId, *success);
}

/*
    Code - 0x06
    Function - REPLY_GETPARAM

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - operation success status
        * PARAMETER_2_STACK_OFFSET - operation result
    
    In case 'success' value is 'false'
    'result' value is undefined
*/
void k_scr_replyGetParam(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const bool, success, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const size_t, result, PARAMETER_2_STACK_OFFSET);

    extern void k_scr_drv_replyGetParam(const procId_t, const bool, const size_t);
    k_scr_drv_replyGetParam(procId, *success, *result);
}

/*
    Code - 0x07
    Function - REPLY_SETPARAM
    
    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - operation success status
*/
void k_scr_replySetParam(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const bool, success, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_drv_replySetParam(const procId_t, const bool);
    k_scr_drv_replySetParam(procId, *success);
}

/*
    Code - 0x08
    Function - REPLY_MEMWRITE
    
    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - operation success status
*/
void k_scr_replyMemWrite(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const bool, success, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_drv_replyMemWrite(const procId_t, const bool);
    k_scr_drv_replyMemWrite(procId, *success);
}
