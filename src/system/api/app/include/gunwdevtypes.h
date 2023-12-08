//
//  gunwdevtypes.h
//  GunwOS
//
//  Created by Artur Danielewski on 31.03.2023.
//

#ifndef GUNWOS_GUNWDEVTYPES_H
#define GUNWOS_GUNWDEVTYPES_H

#include <types.h>

/*
    Device type
*/
enum gnwDeviceType {
    DEV_TYPE_SYSTEM     = (1 << 0),
    DEV_TYPE_MEM        = (1 << 1),
    DEV_TYPE_KEYBOARD   = (1 << 2),
    DEV_TYPE_MOUSE      = (1 << 3),
    DEV_TYPE_DISPLAY    = (1 << 4),
    DEV_TYPE_CHAR_IN    = (1 << 5),
    DEV_TYPE_CHAR_OUT   = (1 << 6),
    DEV_TYPE_STORAGE    = (1 << 7),
    DEV_TYPE_FDC        = (1 << 8) | DEV_TYPE_STORAGE,
    DEV_TYPE_NONE       = 0
};

enum gnwDeviceError {
    GDE_NONE = 0,
    GDE_BUSY,
    GDE_ID_INVALID,
    GDE_NOT_FOUND,
    GDE_CANNOT_BE_HELD,
    GDE_ALREADY_HELD,
    GDE_ALREADY_SET,
    GDE_HANDLE_INVALID,
    GDE_LISTENER_INVALID,
    GDE_DECODER_INVALID,
    GDE_INVALID_DEVICE_STATE,
    GDE_INVALID_OPERATION,
    GDE_INVALID_PARAMETER,
    GDE_OPERATION_FAILED,
    GDE_UNKNOWN = -1
};
_Static_assert(sizeof(enum gnwDeviceError) == sizeof(int_32), "Unexpected enum gnwDeviceError size");

struct gnwDeviceParamDescriptor {
    size_t param;
    size_t subParam;
    size_t paramIndex;
};

#endif // GUNWOS_GUNWDEVTYPES_H
