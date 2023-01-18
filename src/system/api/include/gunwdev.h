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
    GDE_NOT_FOUND,
    GDE_CANNOT_BE_HELD,
    GDE_ALREADY_HELD,
    GDE_UNKNOWN
};

/*
    Requests installation of given hardware driver

    Params:
        * id - device identifier 
               to be returned if the installation succeeds
        * desc - device descriptor (see gunwdrv.h)
*/
static inline enum gnwDriverError devInstall(size_t * const id, const struct gnwDeviceDescriptor * const desc) {
    SYSCALL_PAR1(id);
    SYSCALL_PAR2(desc);

    SYSCALL_FUNC(DEV_INSTALL);
    SYSCALL_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests start of given hardware driver

    Params:
        * id - device identifier
*/
static inline enum gnwDriverError devStart(size_t id) {
    SYSCALL_PAR1(id);

    SYSCALL_FUNC(DEV_START);
    SYSCALL_INT;

    SYSCALL_RETVAL(32);
}

/*
    Requests device information for given type

    Params:
        * type - type of the device (see gunwdrv.h)
        * desc - address of the result description (see gunwdrv.h)
*/
static inline enum gnwDeviceError devGet(const enum gnwDeviceType type, struct gnwDeviceUHADesc * const desc) {
    if (!desc) {
        fug(NULLPTR);
        __builtin_unreachable();
    }

    SYSCALL_PAR1(type);
    SYSCALL_PAR2(desc);

    SYSCALL_FUNC(DEV_GET);
    SYSCALL_INT;

    SYSCALL_RETVAL(32);
}

/*
    Attempts to take hold of the device

    Params:
        * type - device type (see gunwdrv.h)

    Note: In order to use the device
          the process has to be exclusive holder of it
*/
static inline enum gnwDeviceError devAcquire(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_FUNC(DEV_ACQUIRE);
    SYSCALL_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWSCL_H
