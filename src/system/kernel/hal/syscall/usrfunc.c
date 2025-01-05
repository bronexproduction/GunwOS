//
//  usrfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include "params.h"
#include <_gunwctrl.h>
#include <_gunwipc.h>
#include <_gunwmem.h>
#include <_gunwdev.h>
#include <error/fug.h>
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
void k_scr_start(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(addr_t, descPtr, PARAMETER_1_STACK_OFFSET);

    extern void k_scr_usr_start(const procId_t procId, const struct gnwCtrlStartDescriptor * const);
    k_que_dispatch_arch2((fPtr_arch2)(ptr_t)k_scr_usr_start, procId, *descPtr);
}

/*
    Code - 0x01
    Function - LOG

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - message pointer
        * PARAMETER_2_STACK_OFFSET - message size in bytes
*/
void k_scr_log(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const char * const, msgPtr, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const size_t, msgBytes, PARAMETER_2_STACK_OFFSET);

    extern void k_scr_usr_log(const procId_t, const char * const, const size_t);
    k_scr_usr_log(procId, *msgPtr, *msgBytes);
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
void k_scr_devCharWrite(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const char, character, PARAMETER_2_STACK_OFFSET);

    SAFE_STACK_RESULT_ARCH_VAL = k_dev_writeChar(procId, *devId, *character);  
}

/*
    Code - 0x03
    Function - BYE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - status

*/
void k_scr_bye(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const int_32, status, PARAMETER_1_STACK_OFFSET);
    
    extern void k_scr_usr_bye(procId_t, int_32);
    k_scr_usr_bye(procId, *status);
}

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT
*/
void k_scr_waitForEvent(const procId_t procId, const ptr_t refEsp) {
    extern void k_scr_usr_waitForEvent(procId_t);
    k_que_dispatch_arch((fPtr_arch)k_scr_usr_waitForEvent, procId);
}

/*
    Code - 0x05
    Function - TIME_MS

    Return (process stack offset):
        * RESULT_STACK_OFFSET - time in milliseconds
*/
void k_scr_timeMs(const procId_t procId, const ptr_t refEsp) {
    extern time_t k_tmr_getMs();
    SAFE_STACK_RESULT_ARCH_VAL = k_tmr_getMs();
}

/*
    Code - 0x06
    Function - IPC_SEND

    Params (process stack offset): 
        * PARAMETER_1_STACK_OFFSET - IPC sender query pointer relative to caller process memory

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code on failure, GIPCE_NONE otherwise
*/
void k_scr_ipcSend(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const struct gnwIpcSenderQuery * const, queryPtr, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcSend(const procId_t procId, const struct gnwIpcSenderQuery * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_ipcSend(procId, *queryPtr);
}

/*
    Code - 0x07
    Function - IPC_REGISTER

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - IPC handler descriptor pointer (see struct gnwIpcHandlerDescriptor) relative to caller process memory

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code on failure, GIPCE_NONE otherwise
*/
void k_scr_ipcRegister(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const struct gnwIpcHandlerDescriptor * const, descPtr, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcRegister(const procId_t procId, const struct gnwIpcHandlerDescriptor * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_ipcRegister(procId, *descPtr);
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
void k_scr_devGetById(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, id, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(struct gnwDeviceUHADesc * const, descPtr, PARAMETER_2_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devGetById(const procId_t, const size_t, struct gnwDeviceUHADesc * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_devGetById(procId, *id, *descPtr);
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
void k_scr_devGetByType(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const enum gnwDeviceType, type, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(struct gnwDeviceUHADesc * const, descPtr, PARAMETER_2_STACK_OFFSET);
    
    extern enum gnwDeviceError k_scr_usr_devGetByType(const procId_t, const enum gnwDeviceType, struct gnwDeviceUHADesc * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_devGetByType(procId, *type, *descPtr);
}

/*
    Code - 0x0a
    Function - DEV_ACQUIRE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier

    Return (process stack offset):
        * RESULT_STACK_OFFSET - error code (enum gnwDeviceError)
*/
void k_scr_devAcquire(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);

    SAFE_STACK_RESULT_ARCH_VAL = k_dev_acquireHold(procId, *devId);
}

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device identifier
*/
void k_scr_devRelease(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);

    k_dev_releaseHold(procId, *devId);
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
void k_scr_devMemWrite(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const ptr_t, buf, PARAMETER_2_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const range_addr_t * const, rangePtr, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devMemWrite(const procId_t, const size_t devId, const ptr_t buf, const range_addr_t * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_devMemWrite(procId, *devId, *buf, *rangePtr);
}

/*
    Code - 0x0d
    Function - FUG

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - FUG code
*/
void k_scr_fug(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const enum gnwFugCode, code, PARAMETER_1_STACK_OFFSET);

    k_err_fug(procId, *code);
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
void k_scr_devListen(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const gnwDeviceEventListener, lsnr, PARAMETER_2_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const gnwDeviceEventDecoder, decoder, PARAMETER_3_STACK_OFFSET);

    SAFE_STACK_RESULT_ARCH_VAL = k_dev_listen(procId, *devId, *lsnr, *decoder);
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
void k_scr_devGetParam(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const struct gnwDeviceParamDescriptor * const, paramDesc, PARAMETER_2_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(size_t * const, resultPtr, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devGetParam(const procId_t, const size_t, const struct gnwDeviceParamDescriptor * const, size_t * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_devGetParam(procId, *devId, *paramDesc, *resultPtr);
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
void k_scr_devSetParam(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, devId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const struct gnwDeviceParamDescriptor * const, paramDesc, PARAMETER_2_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const size_t, paramVal, PARAMETER_3_STACK_OFFSET);

    extern enum gnwDeviceError k_scr_usr_devSetParam(const procId_t, const size_t, const struct gnwDeviceParamDescriptor * const, const size_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_devSetParam(procId, *devId, *paramDesc, *paramVal);
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
void k_scr_runLoopGetItem(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(struct gnwRunLoopDispatchItem * const, itemPtr, PARAMETER_1_STACK_OFFSET);

    extern enum gnwRunLoopError k_scr_usr_runLoopGetItem(const procId_t, struct gnwRunLoopDispatchItem * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_runLoopGetItem(procId, *itemPtr);
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
void k_scr_runLoopGetData(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const ptr_t, dataBufferPtr, PARAMETER_1_STACK_OFFSET);

    extern enum gnwRunLoopError k_scr_usr_runLoopGetData(const procId_t, ptr_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_runLoopGetData(procId, *dataBufferPtr);
}

/*
    Code - 0x13
    Function - IPC_REPLY

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - IPC reply info pointer
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwIpcError code if any, GIPCE_NONE otherwise
*/
void k_scr_ipcReply(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const struct gnwIpcReplyInfo * const, infoPtr, PARAMETER_1_STACK_OFFSET);

    extern enum gnwIpcError k_scr_usr_ipcReply(const procId_t, const struct gnwIpcReplyInfo * const);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_ipcReply(procId, *infoPtr);
}

/*
    Code - 0x14
    Function - YIELD
*/
void k_scr_yield(const procId_t procId, const ptr_t refEsp) {
    /*
        Change execution to another process
    */
    k_proc_schedule_processStateDidChange();
}

/*
    Code - 0x15
    Function - MEM_PLZ

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - number of pages requested
        * PARAMETER_2_STACK_OFFSET - virtual memory start address (must be aligned to page size)
    
    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwMemoryError code if any, GME_NONE otherwise
*/
void k_scr_memPlz(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const size_t, pageCount, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(const addr_t, vStart, PARAMETER_2_STACK_OFFSET);

    extern enum gnwMemoryError k_scr_usr_memPlz(const procId_t, const size_t, const addr_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_memPlz(procId, *pageCount, *vStart);
}

/*
    Code - 0x16
    Function - MEM_THX

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - virtual memory page start address (must be aligned to page size)

    Return (process stack offset):
        * RESULT_STACK_OFFSET - enum gnwMemoryError code if any, GME_NONE otherwise
*/
void k_scr_memThx(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const addr_t, vStart, PARAMETER_1_STACK_OFFSET);

    extern enum gnwMemoryError k_scr_usr_memThx(const procId_t, const addr_t);
    SAFE_STACK_RESULT_ARCH_VAL = k_scr_usr_memThx(procId, *vStart);
}


/*
    Code - 0x17
    Function - DEV_INIT

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device operator process ID
        * PARAMETER_2_STACK_OFFSET - error pointer (address in caller process space)
*/
void k_scr_devInit(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const procId_t, operatorProcId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(enum gnwDeviceError *, vErrorPtr, PARAMETER_2_STACK_OFFSET);

    extern void k_scr_usr_devInit(const procId_t procId, const procId_t operatorProcId, enum gnwDeviceError * vErrorPtr);
    k_que_dispatch_arch3((fPtr_arch3)(ptr_t)k_scr_usr_devInit, procId, *operatorProcId, (addr_t)*vErrorPtr);
}

/*
    Code - 0x18
    Function - DEV_START

    Params (process stack offset):
        * PARAMETER_1_STACK_OFFSET - device operator process ID
        * PARAMETER_2_STACK_OFFSET - error pointer (address in caller process space)
*/
void k_scr_devStart(const procId_t procId, const ptr_t refEsp) {
    SAFE_STACK_VAL_PTR(const procId_t, operatorProcId, PARAMETER_1_STACK_OFFSET);
    SAFE_STACK_VAL_PTR(enum gnwDeviceError *, vErrorPtr, PARAMETER_2_STACK_OFFSET);

    extern void k_scr_usr_devStart(const procId_t procId, const procId_t operatorProcId, enum gnwDeviceError * vErrorPtr);
    k_que_dispatch_arch3((fPtr_arch3)(ptr_t)k_scr_usr_devStart, procId, *operatorProcId, (addr_t)*vErrorPtr);
}