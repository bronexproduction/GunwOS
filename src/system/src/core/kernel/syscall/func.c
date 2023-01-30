//
//  func.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.01.2021.
//

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/

#include "../hal/io/bus.h"
#include <stdgunw/utils.h>
#include <gunwdev.h>
#include <stdgunw/string.h>
#include "../error/fug.h"

#define SCR_END {__asm__ volatile ("ret");};
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; SCR_END }

/*
    Driver-level system calls
*/

/*
    Code - 0x01
    Function - RDB

    Params:
        * BX - port
        
    Return:
        * AL - value read from bus port
*/
SCR(rdb,
    REG(16, port, bx)
    REG(8, value, al)

    value = k_bus_inb(port);
)

/*
    Code - 0x02
    Function - WRB

    Params:
    
        * BX - port
        * CL - value

*/
SCR(wrb,
    REG(16, port, bx)
    REG(8, value, cl)

    k_bus_outb(port, value);
)

/*
    User-level system calls
*/

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
    Code - 0x06
    Function - DEV_INSTALL

    Params:
        * EBX - device identifier return pointer
        * ECX - device descriptor (struct gnwDeviceDescriptor *)

    Return:
        * EAX - error code (enum gnwDriverError)

    Note:
        * Not allowed from user-level
*/
SCR(devInstall,
    REG(32, id, ebx)
    REG(32, desc, ecx)

    REG_RET(32, err)

    enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const);
    err = k_dev_install((size_t * const)id, (struct gnwDeviceDescriptor*)desc);
)

/*
    Code - 0x07
    Function - DEV_START

    Params:
        * EBX - device identifier

    Return:
        * EAX - error code (enum gnwDriverError)

    Note:
        * Not allowed from user-level
*/
SCR(devStart,
    REG(32, id, ebx)

    REG_RET(32, err)

    enum gnwDriverError k_dev_start(size_t id);
    err = k_dev_start((size_t)id);
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

    size_t procId = 0; // TODO: get caller process id

    REG_RET(32, err)

    enum gnwDeviceError k_dev_acquireHold(size_t, size_t);
    err = k_dev_acquireHold((size_t)procId, (size_t)devId);
)

/*
    Code - 0x0b
    Function - DEV_RELEASE

    Params:
        * EBX - device identifier
*/
SCR(devRelease,
    REG(32, devId, ebx)

    size_t procId = 0; // TODO: get caller process id

    void k_dev_releaseHold(size_t, size_t);
    k_dev_releaseHold((size_t)procId, (size_t)devId);
)

/*
    Code - 0x0c
    Function - DEV_WRITE

    Params:
        * EBX - device identifier
        * ECX - data buffer
*/
SCR(devWrite,
    REG(32, devId, ebx)
    REG(32, buf, ecx)

    REG_RET(32, err)

    size_t procId = 0; // TODO: get caller process id

    enum gnwDeviceError k_dev_write(const size_t,
                                    const size_t,
                                    const void * const);
    err = k_dev_write((const size_t)procId, (const size_t)devId, (const void * const)buf);                                        
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
