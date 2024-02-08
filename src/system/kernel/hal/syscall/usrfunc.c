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
    PAR_PTR_1(descPtr)

    extern void k_scr_usr_start(const procId_t, const struct gnwCtrlStartDescriptor * const);
    k_que_dispatch_arch_arch((fPtr_arch_arch)(ptr_t)k_scr_usr_start, k_proc_getCurrentId(), *descPtr);
)

/*
    Code - 0x01
    Function - LOG

    Params:
        * EBX - message pointer
        * ECX - message size in bytes
*/
SCR(log,
    PAR_PTR_1(msgPtr)
    PAR_PTR_2(msgBytes)

    extern void k_scr_usr_log(const char * const, const size_t);
    k_scr_usr_log((char *)*msgPtr, *msgBytes);
)

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
    PAR_PTR_1(devId);
    PAR_PTR_2(character);

    REG_RET(32, err)

    err = k_dev_writeChar(k_proc_getCurrentId(), (const size_t)*devId, (const char)*character);  
)

/*
    Code - 0x03
    Function - BYE

    Params:
        * EBX - status

*/
SCR(bye,
    PAR_PTR_1(status)
    
    extern void k_scr_usr_bye(procId_t, int_32);
    k_scr_usr_bye(k_proc_getCurrentId(), *status);
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
    PAR_PTR_1(queryPtr)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const);
    err = k_scr_usr_ipcSend((struct gnwIpcSenderQuery *)*queryPtr);
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
    PAR_PTR_1(desc)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * const);
    err = k_scr_usr_ipcRegister((struct gnwIpcHandlerDescriptor *)*desc);
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
    PAR_PTR_1(id)
    PAR_PTR_2(desc)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetById(const size_t, struct gnwDeviceUHADesc * const);
    err = k_scr_usr_devGetById((size_t)*id, (struct gnwDeviceUHADesc *)*desc);
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
    PAR_PTR_1(type)
    PAR_PTR_2(desc)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetByType(const enum gnwDeviceType, struct gnwDeviceUHADesc * const);
    err = k_scr_usr_devGetByType((enum gnwDeviceType)*type, (struct gnwDeviceUHADesc * const)*desc);
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
    PAR_PTR_1(devId)

    REG_RET(32, err)

    err = k_dev_acquireHold(k_proc_getCurrentId(), (size_t)*devId);
)

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params:
        * EBX - device identifier
*/
SCR(devRelease,
    PAR_PTR_1(devId)

    k_dev_releaseHold(k_proc_getCurrentId(), (size_t)*devId);
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
    PAR_PTR_1(devId)
    PAR_PTR_2(buf)
    PAR_PTR_3(rangePtr)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const ptr_t buf, const range_addr_t * const);
    err = k_scr_usr_devMemWrite(*devId, (ptr_t)*buf, (range_addr_t *)*rangePtr);
)

/*
    Code - 0x0d
    Function - FUG

    Params:
        * EBX - FUG code
*/
SCR(fug,
    PAR_PTR_1(code)

    k_err_fug(k_proc_getCurrentId(), *code);
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
    PAR_PTR_1(devId)
    PAR_PTR_2(lsnr)
    PAR_PTR_3(decoder)

    REG_RET(32, err)

    err = k_dev_listen(k_proc_getCurrentId(), (const size_t)*devId, (gnwDeviceEventListener)*lsnr, (gnwDeviceEventDecoder)*decoder);
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
    PAR_PTR_1(devId)
    PAR_PTR_2(paramDesc)
    PAR_PTR_3(resultPtr)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devGetParam(const size_t, const struct gnwDeviceParamDescriptor * const, size_t * const);
    err = k_scr_usr_devGetParam((size_t)*devId, (struct gnwDeviceParamDescriptor *)*paramDesc, (size_t *)*resultPtr);
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
    PAR_PTR_1(devId)
    PAR_PTR_2(paramDesc)
    PAR_PTR_3(paramVal)

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devSetParam(const size_t, const struct gnwDeviceParamDescriptor * const, const size_t);
    err = k_scr_usr_devSetParam((size_t)*devId, (struct gnwDeviceParamDescriptor *)*paramDesc, (size_t)*paramVal);
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
    PAR_PTR_1(itemPtr)

    REG_RET(32, err)

    extern enum gnwRunLoopError k_scr_usr_runLoopGetItem(struct gnwRunLoopDispatchItem * const);
    err = k_scr_usr_runLoopGetItem((struct gnwRunLoopDispatchItem *)*itemPtr);
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
    PAR_PTR_1(dataBufferPtr)

    REG_RET(32, err)

    extern enum gnwRunLoopError k_scr_usr_runLoopGetData(ptr_t);
    err = k_scr_usr_runLoopGetData((ptr_t)*dataBufferPtr);
)

/*
    Code - 0x13
    Function - IPC_REPLY

    Params:
        * EBX - IPC reply info pointer
    
    Return:
        * EAX - enum gnwIpcError code if any, GIPCE_NONE otherwise
*/
SCR(ipcReply,
    PAR_PTR_1(infoPtr)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcReply(const struct gnwIpcReplyInfo * const);
    err = k_scr_usr_ipcReply((struct gnwIpcReplyInfo *)*infoPtr);
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
