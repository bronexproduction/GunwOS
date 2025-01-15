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
    GDE_PRECONDITION_NOT_SATISFIED,
    GDE_OPERATION_FAILED,
    GDE_OPERATION_PENDING,
    GDE_NOT_RESPONDING,
    GDE_UNKNOWN = -1
};
_Static_assert(sizeof(enum gnwDeviceError) == sizeof(int_32), "Unexpected enum gnwDeviceError size");

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

struct gnwDeviceGetParamQuery {
    size_t param;
    size_t subParam;
    size_t paramIndex;
};

struct gnwDeviceSetParamQuery {
    size_t param;
    size_t subParam;
    size_t paramIndex;
    size_t value;
};

struct gnwDeviceMemWriteQuery {
    ptr_t buffer;
    range_addr_t inputBufferRange;
};
/*
            * Buffer pointer (see struct gnwDeviceUHA_mem_desc for parameters)
            * Range of the buffer, relative to (0 - desc.maxInputSizeBytes)
*/

typedef __attribute__((cdecl)) void (*gnwDeviceEventListener)(const struct gnwDeviceEvent * const);

typedef void (*gnwDeviceEventDecoder)(const ptr_t, struct gnwDeviceEvent * const);

#endif // GUNWOS_GUNWDEVTYPES_H
