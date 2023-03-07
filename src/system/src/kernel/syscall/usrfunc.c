//
//  usrfunc.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

#include <stdgunw/utils.h>
#include <stdgunw/string.h>
#include <gunwdev.h>
#include "func.h"
#include <error/fug.h>
#include <error/panic.h>
#include <hal/proc/proc.h>
#include <hal/mem/mem.h>
#include <dev/dev.h>

/*
    User-level system calls
*/

/*
    Code - 0x01
    Function - DEBUG_PRINT

    Params:
        * EBX - null-terminated character array pointer

    Return:
        * EAX - number of bytes written
*/
static size_t debugPrint(uint_32 buffer) {
    // check buffer bounds
    // there might be (must be) a better way to do it
    struct k_mem_zone procZone = k_mem_zoneForProc(k_proc_getCurrentId());
    if (buffer >= procZone.sizeBytes) {
        OOPS("Access violation");
    }
    buffer += (uint_32)procZone.startPtr;
    size_t bufLen = strlen((const char * const)buffer);
    if ((buffer + bufLen) > (size_t)procZone.endPtr) {
        OOPS("Access violation");   
    }

    extern int user_cli_putsl(const char * const s, unsigned int l);
    user_cli_putsl((const char * const)buffer, bufLen);
    return bufLen;
}

SCR(debugPrint,
    // Buffer address (relative to process memory)
    REG(32, buffer, ebx)

    REG_RET(32, bytesWritten)

    bytesWritten = debugPrint(buffer);
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

    err = k_dev_writeChar((const size_t)k_proc_getCurrentId(), (const size_t)devId, (const char)character);  
)

/*
    Code - 0x03
    Function - EXIT

    Params:
    
        * EBX - status

*/
// TODO: wip
SCR(exit,
    REG(32, status, ebx)
)

/*
    Code - 0x05
    Function - SLEEPMS

    Params:
        * EBX - time in milliseconds
*/
SCR(sleepms,
    REG(32, timems, ebx)

    extern void k_tmr_sleepms(const unsigned int);
    k_tmr_sleepms(timems);
)

/*
    Code - 0x08
    Function - DEV_GET_BY_ID

    Params:
        * EBX - device identifier
        * ECX - device descriptor pointer (struct gnwDeviceUHADesc *)

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devGetById,
    REG(32, id, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    enum gnwDeviceError k_dev_getById(const size_t, struct gnwDeviceUHADesc * const);
    err = k_dev_getById((const size_t)id, (struct gnwDeviceUHADesc * const)desc);
)

/*
    Code - 0x09
    Function - DEV_GET_BY_TYPE

    Params:
        * EBX - device type (enum gnwDeviceType)
        * ECX - device descriptor pointer (struct gnwDeviceUHADesc *)

    Return:
        * EAX - error code (enum gnwDeviceError)
*/
SCR(devGetByType,
    REG(32, type, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    enum gnwDeviceError k_dev_getByType(enum gnwDeviceType, struct gnwDeviceUHADesc * const);
    err = k_dev_getByType((enum gnwDeviceType)type, (struct gnwDeviceUHADesc * const)desc);
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

    err = k_dev_acquireHold((size_t)k_proc_getCurrentId(), (size_t)devId);
)

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params:
        * EBX - device identifier
*/
SCR(devRelease,
    REG(32, devId, ebx)

    k_dev_releaseHold((size_t)k_proc_getCurrentId(), (size_t)devId);
)

/*
    Code - 0x0c
    Function - DEV_MEM_WRITE

    Params:
        * EBX - device identifier
        * ECX - data buffer
*/
SCR(devMemWrite,
    REG(32, devId, ebx)
    REG(32, buf, ecx)

    REG_RET(32, err)

    err = k_dev_writeMem((const size_t)k_proc_getCurrentId(), (const size_t)devId, (const void * const)buf);                                        
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
