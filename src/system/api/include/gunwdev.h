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

/*
    Device type
*/
enum gnwDeviceType {
    DEV_TYPE_SYSTEM,
    DEV_TYPE_KEYBOARD,
    DEV_TYPE_FDC,
    DEV_TYPE_CHAR_IN,
    DEV_TYPE_CHAR_OUT,
    DEV_TYPE_DISPLAY,
    DEV_TYPE_UNKNOWN = -1
};

enum gnwDeviceError {
    GDE_NONE = 0,
    GDE_ID_INVALID,
    GDE_NOT_FOUND,
    GDE_CANNOT_BE_HELD,
    GDE_ALREADY_HELD,
    GDE_ALREADY_SET,
    GDE_HANDLE_INVALID,
    GDE_LISTENER_INVALID,
    GDE_INVALID_DEVICE_STATE,
    GDE_INVALID_OPERATION,
    GDE_OPERATION_FAILED,
    GDE_UNKNOWN = -1
};
_Static_assert(sizeof(enum gnwDeviceError) == sizeof(int_32), "Unexpected enum gnwDeviceError size");

typedef __attribute__((cdecl)) void (*gnwDeviceEventListener_void)(int_32 type);
typedef __attribute__((cdecl)) void (*gnwDeviceEventListener_u8)(int_32 type, uint_8 data);

union gnwDeviceEventListener {
    uint_32 _handle;
    gnwDeviceEventListener_void onEvent_void;
    gnwDeviceEventListener_u8 onEvent_u8;
};
_Static_assert(sizeof(union gnwDeviceEventListener) == sizeof(uint_32), "Unexpected union gnwDeviceEventListener size");

/*
    Requests device information for given id

    Params:
        * id - id of the device
        * desc - address of the result description (see gunwdrv.h)
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
    Attempts to take hold of the device

    Params:
        * identifier - device identifier

    Note: In order to use the device
          the process has to be exclusive holder of it
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
SYSCALL_DECL enum gnwDeviceError devCharWrite(uint_32 deviceId, 
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
                                             const void * const buffer) {
    CHECKPTR(buffer);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(buffer);

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
                                           const union gnwDeviceEventListener listener) {
    CHECKPTR(listener._handle);
    
    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(listener._handle);

    SYSCALL_USER_FUNC(DEV_LISTEN);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWDEV_H
