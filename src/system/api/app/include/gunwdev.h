//
//  gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDEV_H
#define GUNWOS_GUNWDEV_H

#include "scl_user.h"
#include "gunwfug.h"
#include "gunwuhadesc.h"
#include "gunwevent.h"

struct gnwDeviceEvent {
    /*
        Event type (device specific)
    */
    int_32 type;

    /*
        Event data pointer
    */
    ptr_t data;

    /*
        Event data size in bytes
    */
    size_t dataSizeBytes;
};

typedef __attribute__((cdecl)) void (*gnwDeviceEventListener)(const struct gnwDeviceEvent * const);

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
        * paramDescriptor - device-specific parameter info (see UHA for a specific device)
        * result - pointer to the result buffer
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devGetParam(const size_t deviceId,
                                             const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                             size_t * const result) {
    CHECKPTR(paramDescriptor);
    CHECKPTR(result);

    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(paramDescriptor);
    SYSCALL_PAR3(result);

    SYSCALL_USER_FUNC(DEV_GET_PARAM);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Sets parameter value for given device ID

    Params:
        * id - id of the device
        * paramDescriptor - device-specific parameter info (see UHA for a specific device)
        * value - value to be set for parameter
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)

    Note: In order to set device parameters
          the process has to be exclusive holder of it
*/
SYSCALL_DECL enum gnwDeviceError devSetParam(const size_t deviceId,
                                             const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                             const size_t value) {
    CHECKPTR(paramDescriptor);

    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(paramDescriptor);
    SYSCALL_PAR3(value);

    SYSCALL_USER_FUNC(DEV_SET_PARAM);
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
        * devInputBufferRange - Range of bytes in relation to accepted input memory range
    
    Return value: Device error code or GDE_NONE (see enum gnwDeviceError)
*/
SYSCALL_DECL enum gnwDeviceError devMemWrite(const size_t identifier,
                                             const ptr_t const buffer,
                                             const range_addr_t * const devInputBufferRange) {
    CHECKPTR(buffer);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(buffer);
    SYSCALL_PAR3(devInputBufferRange);

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
                                           const gnwDeviceEventListener listener) {
    CHECKPTR(listener);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(listener);

    SYSCALL_USER_FUNC(DEV_LISTEN);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWDEV_H
