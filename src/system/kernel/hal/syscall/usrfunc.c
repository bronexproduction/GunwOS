//
//  usrfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include "params.h"
#include <src/_gunwctrl.h>
#include <src/_gunwipc.h>
#include <error/fug.h>
#include <hal/proc/proc.h>
#include <schedule/schedule.h>
#include <dev/dev.h>
#include <ipc/ipc.h>
#include <queue/queue.h>
#include <error/panic.h>
#include <runloop/runloop.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - path to start descriptor pointer (struct gnwCtrlStartDescriptor) relative to caller process memory
*/
void k_scr_start(const ptr_t refEsp) {
    const addr_t * const descPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_usr_start(const procId_t procId, const struct gnwCtrlStartDescriptor * const);
    k_que_dispatch_arch_arch((fPtr_arch_arch)(ptr_t)k_scr_usr_start, k_proc_getCurrentId(), *descPtr);
}

/*
    Code - 0x01
    Function - LOG

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - message pointer
        * PARAMETER_2_STACK_OFFSET - message size in bytes
*/
void k_scr_log(const ptr_t refEsp) {
    const char * const * const msgPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const size_t * const msgBytes = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);

    extern void k_scr_usr_log(const procId_t, const char * const, const size_t);
    k_scr_usr_log(k_proc_getCurrentId(), *msgPtr, *msgBytes);
}

/*
    Code - 0x02
    Function - DEV_CHAR_WRITE
    
    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - character output device identifier
        * PARAMETER_2_STACK_OFFSET - character to be written

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (see enum gnwDeviceError)
*/
void k_scr_devCharWrite(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const char * const character = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);

    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_dev_writeChar(k_proc_getCurrentId(), *devId, *character);  
}

/*
    Code - 0x03
    Function - BYE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - status

*/
void k_scr_bye(const ptr_t refEsp) {
    const int_32 * const status = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    
    extern void k_scr_usr_bye(procId_t, int_32);
    k_scr_usr_bye(k_proc_getCurrentId(), *status);
}

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT
*/
void k_scr_waitForEvent(const ptr_t refEsp) {
    extern void k_scr_usr_waitForEvent(procId_t);
    k_que_dispatch_arch((fPtr_arch)k_scr_usr_waitForEvent, k_proc_getCurrentId());
}

/*
    Code - 0x05
    Function - TIME_MS

    Return (process stack offset):
        * RESULT_STACK_OFFSET - time in milliseconds
*/
void k_scr_timeMs(const ptr_t refEsp) {
    extern time_t k_tmr_getMs();
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_tmr_getMs();
}

/*
    Code - 0x06
    Function - IPC_SEND

    Params (process stack offset): 
        * PARAMETER_1_STACK_OFFSET - IPC sender query pointer relative to caller process memory

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code on failure, GIPCE_NONE otherwise
*/
void k_scr_ipcSend(const ptr_t refEsp) {
    const struct gnwIpcSenderQuery * const * const queryPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcSend(const procId_t procId, const struct gnwIpcSenderQuery * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_ipcSend(k_proc_getCurrentId(), *queryPtr);
}

/*
    Code - 0x07
    Function - IPC_REGISTER

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - IPC handler descriptor pointer (see struct gnwIpcHandlerDescriptor) relative to caller process memory

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code on failure, GIPCE_NONE otherwise
*/
void k_scr_ipcRegister(const ptr_t refEsp) {
    const struct gnwIpcHandlerDescriptor * const * const desc = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcRegister(const procId_t procId, const struct gnwIpcHandlerDescriptor * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_ipcRegister(k_proc_getCurrentId(), *desc);
}

/*
    Code - 0x08
    Function - DEV_GET_BY_ID

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
        * PARAMETER_2_STACK_OFFSET - device descriptor pointer relative to caller process memory (struct gnwDeviceUHADesc *)

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devGetById(const ptr_t refEsp) {
    const size_t * const id = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    struct gnwDeviceUHADesc * const * const desc = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devGetById(const procId_t, const size_t, struct gnwDeviceUHADesc * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_devGetById(k_proc_getCurrentId(), *id, *desc);
}

/*
    Code - 0x09
    Function - DEV_GET_BY_TYPE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device type (enum gnwDeviceType)
        * PARAMETER_2_STACK_OFFSET - device descriptor pointer relative to caller process memory (struct gnwDeviceUHADesc *)

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devGetByType(const ptr_t refEsp) {
    const enum gnwDeviceType * const type = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    struct gnwDeviceUHADesc * const * const desc = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);
    
    extern enum gnwDeviceError k_scr_usr_devGetByType(const procId_t, const enum gnwDeviceType, struct gnwDeviceUHADesc * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_devGetByType(k_proc_getCurrentId(), *type, *desc);
}

/*
    Code - 0x0a
    Function - DEV_ACQUIRE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devAcquire(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_dev_acquireHold(k_proc_getCurrentId(), *devId);
}

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
*/
void k_scr_devRelease(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    k_dev_releaseHold(k_proc_getCurrentId(), *devId);
}

/*
    Code - 0x0c
    Function - DEV_MEM_WRITE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
        * PARAMETER_2_STACK_OFFSET - data buffer relative to caller process memory 
        * PARAMETER_3_STACK_OFFSET - memory range (pointer) relative to device memory 

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devMemWrite(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const ptr_t * const buf = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);
    const range_addr_t * const * const rangePtr = userStackSafeValuePointer(refEsp, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devMemWrite(const procId_t, const size_t devId, const ptr_t buf, const range_addr_t * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_devMemWrite(k_proc_getCurrentId(), *devId, *buf, *rangePtr);
}

/*
    Code - 0x0d
    Function - FUG

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - FUG code
*/
void k_scr_fug(const ptr_t refEsp) {
    const enum gnwFugCode * const code = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    k_err_fug(k_proc_getCurrentId(), *code);
}

/*
    Code - 0x0e
    Function - DEV_LISTEN

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
        * PARAMETER_2_STACK_OFFSET - listener (gnwDeviceEventListener)
        * PARAMETER_3_STACK_OFFSET - decoder (gnwDeviceEventDecoder)
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devListen(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const gnwDeviceEventListener * const lsnr = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);
    const gnwDeviceEventDecoder * const decoder = userStackSafeValuePointer(refEsp, PARAMETER_3_STACK_OFFSET);

    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_dev_listen(k_proc_getCurrentId(), *devId, *lsnr, *decoder);
}

/*
    Code - 0x0f
    Function - DEV_GET_PARAM

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
        * PARAMETER_2_STACK_OFFSET - device parameter descriptor
        * PARAMETER_3_STACK_OFFSET - result pointer relative to caller process memory
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devGetParam(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const struct gnwDeviceParamDescriptor * const * const paramDesc = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);
    size_t * const * const resultPtr = userStackSafeValuePointer(refEsp, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devGetParam(const procId_t, const size_t, const struct gnwDeviceParamDescriptor * const, size_t * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_devGetParam(k_proc_getCurrentId(), *devId, *paramDesc, *resultPtr);
}

/*
    Code - 0x10
    Function - DEV_SET_PARAM

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
        * PARAMETER_2_STACK_OFFSET - device parameter descriptor
        * PARAMETER_3_STACK_OFFSET - parameter value
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devSetParam(const ptr_t refEsp) {
    const size_t * const devId = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);
    const struct gnwDeviceParamDescriptor * const * const paramDesc = userStackSafeValuePointer(refEsp, PARAMETER_2_STACK_OFFSET);
    const size_t * const paramVal = userStackSafeValuePointer(refEsp, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devSetParam(const procId_t, const size_t, const struct gnwDeviceParamDescriptor * const, const size_t);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_devSetParam(k_proc_getCurrentId(), *devId, *paramDesc, *paramVal);
}

/*
    Code - 0x11
    Function - RUNLOOP_GET_ITEM

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - struct gnwRunLoopDispatchItem pointer relative to caller process memory
                      (to be filled with unhandled entry)
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwRunLoopError value on failure, GRLE_NONE otherwise
*/
void k_scr_runLoopGetItem(const ptr_t refEsp) {
    struct gnwRunLoopDispatchItem * const * const itemPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern enum gnwRunLoopError k_scr_usr_runLoopGetItem(const procId_t, struct gnwRunLoopDispatchItem * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_runLoopGetItem(k_proc_getCurrentId(), *itemPtr);
}

/*
    Code - 0x12
    Function - RUNLOOP_GET_DATA

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - pointer to entry buffer data relative to caller process memory
                      (to be filled with unhandled entry data)
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwRunLoopError code if any, GRLE_NONE otherwise
*/
void k_scr_runLoopGetData(const ptr_t refEsp) {
    const ptr_t * const dataBufferPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern enum gnwRunLoopError k_scr_usr_runLoopGetData(const procId_t, ptr_t);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_runLoopGetData(k_proc_getCurrentId(), *dataBufferPtr);
}

/*
    Code - 0x13
    Function - IPC_REPLY

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - IPC reply info pointer
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwIpcError code if any, GIPCE_NONE otherwise
*/
void k_scr_ipcReply(const ptr_t refEsp) {
    const struct gnwIpcReplyInfo * const * const infoPtr = userStackSafeValuePointer(refEsp, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcReply(const procId_t, const struct gnwIpcReplyInfo * const);
    *(addr_t *)userStackSafeValuePointer(refEsp, RESULT_STACK_OFFSET) = k_scr_usr_ipcReply(k_proc_getCurrentId(), *infoPtr);
}

/*
    Code - 0x14
    Function - YIELD
*/
void k_scr_yield(const ptr_t refEsp) {
    /*
        Change execution to another process
    */
    k_proc_schedule_processStateDidChange();
}
