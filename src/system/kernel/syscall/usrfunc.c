//
//  usrfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include "func.h"
#include <gunwctrl.h>
#include <gunwipc.h>
#include <error/fug.h>
#include <hal/proc/proc.h>
#include <dev/dev.h>
#include <queue/queue.h>
#include <error/panic.h>

/*
    User-level system calls
*/

/*
    Code - 0x00
    Function - START

    Params:
        * EBX - path to executable - null-terminated character array pointer, relative to caller process memory 
        * ECX - path length in bytes

    Return:
        * EAX - Start error if any, otherwise GCE_NONE (see enum gnwCtrlError)
*/
SCR(start,
    REG(32, path, ebx)
    REG(32, pathLen, ecx)

    REG_RET(32, err)
    
    OOPS("Not supported yet");

    extern enum gnwCtrlError k_scr_usr_start(const char * const, const size_t);
    err = k_scr_usr_start((const char * const)path, (const size_t)pathLen);
)

/*
    Code - 0x01
    Function - DEBUG_PRINT

    Params:
        * EBX - null-terminated character array pointer, relative to caller process memory 
        * ECX - character array buffer length in bytes

    Return:
        * EAX - number of bytes written
*/
SCR(debugPrint,
    // Buffer address (relative to process memory)
    REG(32, buffer, ebx)
    REG(32, bufferLen, ecx)

    REG_RET(32, bytesWritten)

    extern size_t k_scr_usr_debugPrint(const char * const, const size_t);
    bytesWritten = k_scr_usr_debugPrint((const char * const)buffer, (const size_t)bufferLen);
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
    k_proc_stop(procId);
)

/*
    Code - 0x04
    Function - WAIT_FOR_EVENT

    Params:
        * EBX - run loop (const struct gnwRunLoop * const), relative to caller process memory
*/
SCR(waitForEvent,
    REG(32, rlp, ebx)

    extern void k_scr_usr_waitForEvent(addr_t, addr_t);
    k_que_dispatch_arch_arch(k_scr_usr_waitForEvent, k_proc_getCurrentId(), rlp)
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
        * EBX - IPC path
        * ECX - IPC path length
        * DL - character to be sent

    Return:
        * EAX - error code on failure, GIPCE_NONE otherwise
*/
SCR(ipcSend,
    REG(32, path, ebx)
    REG(32, pathLen, ecx)
    REG(8, c, dl)

    REG_RET(32, err)

    extern enum gnwIpcError k_scr_usr_ipcSend(const char * const, const size_t, const char);
    err = k_scr_usr_ipcSend((char *)path, (size_t)pathLen, (char)c);
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

    REG_RET(32, err)

    extern enum gnwDeviceError k_scr_usr_devMemWrite(const size_t devId, const void * const buf);
    err = k_scr_usr_devMemWrite(devId, (void *)buf);
)

/*
    Code - 0x0d
    Function - FUG

    Params:
        * EBX - FUG code
*/
SCR(fug,
    REG(32, code, ebx)

    k_err_fug(code);
)

/*
    Code - 0x0e
    Function - DEV_LISTEN

    Params:
        * EBX - device identifier
        * ECX - listener (const union gnwEventListener)
        * EDX - run loop (const struct gnwRunLoop * const), relative to caller process memory 
    
    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devListen,
    REG(32, devId, ebx)
    REG(32, lsnr, ecx)
    REG(32, rlp, edx)

    REG_RET(32, err)

    err = k_dev_listen(k_proc_getCurrentId(), (const size_t)devId, (const union gnwEventListener)lsnr, (struct gnwRunLoop *)rlp);
)
