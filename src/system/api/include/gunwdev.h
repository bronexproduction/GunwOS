//
//  gunwdev.h
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef GUNWOS_GUNWDEV_H
#define GUNWOS_GUNWDEV_H

#include "scl_def.h"
#include "gunwdrv.h"
#include "gunwfug.h"

enum gnwDeviceError {
    GDE_NONE = 0,
    GDE_ID_INVALID,
    GDE_NOT_FOUND,
    GDE_CANNOT_BE_HELD,
    GDE_ALREADY_HELD,
    GDE_HANDLE_INVALID,
    GDE_INVALID_DEVICE_STATE,
    GDE_INVALID_OPERATION,
    GDE_UNKNOWN         = -1
};

/*
    Requests installation of given hardware driver

    Params:
        * id - device identifier 
               to be returned if the installation succeeds
        * desc - device descriptor (see gunwdrv.h)
*/
#warning Consider removing - no reason to be part of the API
static inline __attribute__((always_inline)) enum gnwDriverError devInstall(size_t * const id, const struct gnwDeviceDescriptor * const desc) {
    CHECKPTR(id);
    CHECKPTR(desc);

    SYSCALL_PAR1(id);
    SYSCALL_PAR2(desc);

    SYSCALL_DRIVER_FUNC(DEV_INSTALL);
    SYSCALL_DRIVER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests start of given hardware driver

    Params:
        * id - device identifier
*/
#warning Consider removing - no reason to be part of the API
static inline __attribute__((always_inline)) enum gnwDriverError devStart(size_t id) {
    SYSCALL_PAR1(id);

    SYSCALL_DRIVER_FUNC(DEV_START);
    SYSCALL_DRIVER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests device information for given id

    Params:
        * id - id of the device
        * desc - address of the result description (see gunwdrv.h)
*/
static inline __attribute__((always_inline)) enum gnwDeviceError devGetById(const size_t deviceId, struct gnwDeviceUHADesc * const desc) {
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
static inline __attribute__((always_inline)) enum gnwDeviceError devGetByType(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
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
static inline __attribute__((always_inline)) enum gnwDeviceError devAcquire(const uint_32 identifier) {
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
static inline __attribute__((always_inline)) void devRelease(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_USER_FUNC(DEV_RELEASE);
    SYSCALL_USER_INT;
}

/*
    Write to the device

    Params:
        * identifier - device identifier
        * buffer - data buffer pointer
*/
static inline __attribute__((always_inline)) enum gnwDeviceError devWrite(const size_t identifier,
                                           const void * const buffer) {
    CHECKPTR(buffer);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(buffer);

    SYSCALL_USER_FUNC(DEV_WRITE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWSCL_H
