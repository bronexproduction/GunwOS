//
//  usrfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include "func.h"
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
#include <scl_def.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START

    Params:
        * EBX - path to start descriptor pointer (struct gnwCtrlStartDescriptor) relative to caller process memory
*/
SCR(start,
    REG(32, descPtr, ebx)

    extern void k_scr_usr_start(const procId_t, const struct gnwCtrlStartDescriptor * const);
    k_que_dispatch_arch_arch((fPtr_arch_arch)(ptr_t)k_scr_usr_start, k_proc_getCurrentId(), descPtr);
)

/*
    Code - 0x01
    Function - none
*/

/*
    Code - 0x02
    Function - DEV_CHAR_WRITE
    
    Params:
        * EBX - character output device identifier
        * CL - character to be written

    Return:
        * EAX - error code (see enum gnwDeviceError)
*/
SCR(devCharWrite,
    REG(32, devId, ebx);
    REG(8, character, cl);

    REG_RET(32, err)

    err = k_dev_writeChar(k_proc_getCurrentId(), (const size_t)devId, (const char)character);  
)

/*
    Code - 0x03
    Function - EXIT

    Params:
        * EBX - status

*/
SCR(exit,
    REG(32, status, ebx)
    
    const procId_t procId = k_proc_getCurrentId();
    k_que_dispatch_arch((fPtr_arch)k_dev_procCleanup, procId);
    k_que_dispatch_arch((fPtr_arch)k_ipc_procCleanup, procId);
    k_que_dispatch_arch((fPtr_arch)k_runloop_procCleanup, procId);
    k_proc_stop(procId);
)

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT
*/
SCR(waitForEvent,
    extern void k_scr_usr_waitForEvent(procId_t);
    k_que_dispatch_arch((fPtr_arch)k_scr_usr_waitForEvent, k_proc_getCurrentId())
)

/*
    Code - 0x05
    Function - TIME_MS

    Return:
        * EAX - time in milliseconds
*/
SCR(timeMs,
    REG_RET(32, time)

    extern time_t k_tmr_getMs();
    time = k_tmr_getMs();
)

/*
    Code - 0x06
    Function - IPC_SEND

    Params: 
        * EBX - IPC sender query pointer relative to caller process memory

    Return:
        * EAX - error code on failure, GIPCE_NONE otherwise
*/
SCR(ipcSend,
    REG(32, queryPtr, ebx)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const);
    err = k_scr_usr_ipcSend((struct gnwIpcSenderQuery *)queryPtr);
)

/*
    Code - 0x07
    Function - IPC_REGISTER

    Params:
        * EBX - IPC handler descriptor pointer (see struct gnwIpcHandlerDescriptor) relative to caller process memory

    Return:
        * EAX - error code on failure, GIPCE_NONE otherwise
*/
SCR(ipcRegister,
    REG(32, desc, ebx)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * const);
    err = k_scr_usr_ipcRegister((struct gnwIpcHandlerDescriptor *)desc);
)

/*
    Code - 0x08
    Function - DEV_GET_BY_ID

    Params:
        * EBX - device identifier
        * ECX - device descriptor pointer relative to caller process memory (struct gnwDeviceUHADesc *)

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devGetById,
    REG(32, id, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetById(const size_t, struct gnwDeviceUHADesc * const);
    err = k_scr_usr_devGetById((size_t)id, (struct gnwDeviceUHADesc *)desc);
)

/*
    Code - 0x09
    Function - DEV_GET_BY_TYPE

    Params:
        * EBX - device type (enum gnwDeviceType)
        * ECX - device descriptor pointer relative to caller process memory (struct gnwDeviceUHADesc *)

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devGetByType,
    REG(32, type, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetByType(const enum gnwDeviceType, struct gnwDeviceUHADesc * const);
    err = k_scr_usr_devGetByType((enum gnwDeviceType)type, (struct gnwDeviceUHADesc * const)desc);
)

/*
    Code - 0x0a
    Function - DEV_ACQUIRE

    Params:
        * EBX - device identifier

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devAcquire,
    REG(32, devId, ebx)

    REG_RET(32, err)

    err = k_dev_acquireHold(k_proc_getCurrentId(), (size_t)devId);
)

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params:
        * EBX - device identifier
*/
SCR(devRelease,
    REG(32, devId, ebx)

    k_dev_releaseHold(k_proc_getCurrentId(), (size_t)devId);
)

/*
    Code - 0x0c
    Function - DEV_MEM_WRITE

    Params:
        * EBX - device identifier
        * ECX - data buffer relative to caller process memory 

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devMemWrite,
    REG(32, devId, ebx)
    REG(32, buf, ecx)
    REG(32, rangePtr, edx)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const ptr_t buf, const range_addr_t * const);
    err = k_scr_usr_devMemWrite(devId, (ptr_t)buf, (range_addr_t *)rangePtr);
)

/*
    Code - 0x0d
    Function - FUG

    Params:
        * EBX - FUG code
*/
SCR(fug,
    REG(32, code, ebx)

    k_err_fug(k_proc_getCurrentId(), code);
)

/*
    Code - 0x0e
    Function - DEV_LISTEN

    Params:
        * EBX - device identifier
        * ECX - listener (gnwDeviceEventListener)
        * EDX - decoder (gnwDeviceEventDecoder)
    
    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devListen,
    REG(32, devId, ebx)
    REG(32, lsnr, ecx)
    REG(32, decoder, edx)

    REG_RET(32, err)

    err = k_dev_listen(k_proc_getCurrentId(), (const size_t)devId, (gnwDeviceEventListener)lsnr, (gnwDeviceEventDecoder)decoder);
)

/*
    Code - 0x0f
    Function - DEV_GET_PARAM

    Params:
        * EBX - device identifier
        * ECX - device parameter descriptor
        * EDX - result pointer relative to caller process memory
    
    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devGetParam,
    REG(32, devId, ebx)
    REG(32, paramDesc, ecx)
    REG(32, resultPtr, edx)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetParam(const size_t, const struct gnwDeviceParamDescriptor * const, size_t * const);
    err = k_scr_usr_devGetParam((size_t)devId, (struct gnwDeviceParamDescriptor *)paramDesc, (size_t *)resultPtr);
)

/*
    Code - 0x10
    Function - DEV_SET_PARAM

    Params:
        * EBX - device identifier
        * ECX - device parameter descriptor
        * EDX - parameter value
    
    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devSetParam,
    REG(32, devId, ebx)
    REG(32, paramDesc, ecx)
    REG(32, paramVal, edx)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devSetParam(const size_t, const struct gnwDeviceParamDescriptor * const, const size_t);
    err = k_scr_usr_devSetParam((size_t)devId, (struct gnwDeviceParamDescriptor *)paramDesc, (size_t)paramVal);
)

/*
    Code - 0x11
    Function - RUNLOOP_GET_ITEM

    Params:
        * EBX - struct gnwRunLoopDispatchItem pointer relative to caller process memory
                (to be filled with unhandled entry)
    
    Return:
        * EAX - enum gnwRunLoopError value on failure, GRLE_NONE otherwise
*/
SCR(runLoopGetItem,
    REG(32, itemPtr, ebx)

    REG_RET(32, err)

    extern enum gnwRunLoopError k_scr_usr_runLoopGetItem(struct gnwRunLoopDispatchItem * const);
    err = k_scr_usr_runLoopGetItem((struct gnwRunLoopDispatchItem *)itemPtr);
)

/*
    Code - 0x12
    Function - RUNLOOP_GET_DATA

    Params:
        * EBX - pointer to entry buffer data relative to caller process memory
                (to be filled with unhandled entry data)
    
    Return:
        * EAX - enum gnwRunLoopError code if any, GRLE_NONE otherwise
*/
SCR(runLoopGetData,
    REG(32, dataBufferPtr, ebx)

    REG_RET(32, err)

    extern enum gnwRunLoopError k_scr_usr_runLoopGetData(ptr_t);
    err = k_scr_usr_runLoopGetData((ptr_t)dataBufferPtr);
)

/*
    Code - 0x13
    Function - IPC_REPLY

    Params:
        * EBX - pointer to reply buffer data relative to caller process memory
        * ECX - size of reply buffer in bytes
        * EDX - IPC query token
    
    Return:
        * EAX - enum gnwIpcError code if any, GIPCE_NONE otherwise
*/
SCR(ipcReply,
    REG(32, replyBufferPtr, ebx)
    REG(32, replySizeBytes, ecx)
    REG(32, token, edx)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcReply(const ptr_t, const size_t, const size_t);
    err = k_scr_usr_ipcReply((ptr_t)replyBufferPtr, replySizeBytes, token);
)

/*
    Code - 0x14
    Function - YIELD
*/
SCR(yield,
    /*
        Change execution to another process
    */
    k_proc_schedule_processStateDidChange();
)
