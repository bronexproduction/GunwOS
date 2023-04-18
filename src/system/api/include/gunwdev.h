//
//  gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDEV_H
#define GUNWOS_GUNWDEV_H

#include <scl_def.h>
#include <gunwfug.h>
#include <gunwuhadesc.h>
#include <gunwevent.h>
#include <gunwrlp.h>

/*
    Requests device information for given id

    Params:
        * id - id of the device
        * desc - address of the result description (see gunwdrv.h)
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devGetById(const size_t deviceId, 
                                            struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(desc);

    SYSCALL_USER_FUNC(DEV_GET_BY_ID);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests device information for given type

    Params:
        * type - type of the device (see gunwdrv.h)
        * desc - address of the result description (see gunwdrv.h)
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devGetByType(const enum gnwDeviceType type, 
                                              struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_PAR1(type);
    SYSCALL_PAR2(desc);

    SYSCALL_USER_FUNC(DEV_GET_BY_TYPE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests parameter value for given device ID, if any

    Params:
        * id - id of the device
        * param - device-specific parameter code (see UHA for a specific device)
        * paramIndex - parameter result index - in case more than one value are available
        * result - pointer to the result buffer
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devGetParam(const size_t deviceId, 
                                             const uint_16 param,
                                             const uint_16 paramIndex,
                                             size_t * const result) {
    CHECKPTR(result);

    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2((((size_t)param) << 16) | paramIndex);
    SYSCALL_PAR3(result);

    SYSCALL_USER_FUNC(DEV_GET_PARAM);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Attempts to take hold of the device

    Params:
        * identifier - device identifier

    Note: In order to use the device
          the process has to be exclusive holder of it

    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devAcquire(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_USER_FUNC(DEV_ACQUIRE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Releases the hold taken on the device (if any)

    Params:
        * identifier - device identifier
*/
SYSCALL_DECL void devRelease(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_USER_FUNC(DEV_RELEASE);
    SYSCALL_USER_INT;
}

/*
    Write character to character output device

    Parameters:
    * character - Character to be written
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devCharWrite(const uint_32 deviceId, 
                                              const char character) {
    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(character);

    SYSCALL_USER_FUNC(DEV_CHAR_WRITE);
    SYSCALL_USER_INT;
    
    register enum gnwDeviceError ret __asm__ ("eax");
    return ret;
}

/*
    Write to the device

    Params:
        * identifier - device identifier
        * buffer - data buffer pointer
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devMemWrite(const size_t identifier,
                                             const void * const buffer,
                                             const range_addr_t * const devMemRange) {
    CHECKPTR(buffer);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(buffer);
    SYSCALL_PAR3(devMemRange);

    SYSCALL_USER_FUNC(DEV_MEM_WRITE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Register a listener to device events

    Params:
        * identifier - device identifier
        * listener - event listener
*/
SYSCALL_DECL enum gnwDeviceError devListen(const size_t identifier,
                                           const union gnwEventListener listener) {
    CHECKPTR(listener._handle);
    
    ptr_t rlpPtr = runLoopGetMain();
    CHECKPTR(rlpPtr);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(listener._handle);
    SYSCALL_PAR3(rlpPtr);

    SYSCALL_USER_FUNC(DEV_LISTEN);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWDEV_H
