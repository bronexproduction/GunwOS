//
//  gunwdev.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "_gunwdev.h"
#include "../include/gunwdev.h"
#include "../include/gunwfug.h"
#include "scl_user.h"
#include <mem.h>

#ifndef _GUNWAPI_KERNEL

enum gnwDeviceError devGetById(const size_t deviceId,
                               struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_USER_CALL(DEV_GET_BY_ID, deviceId, desc, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devGetByType(const enum gnwDeviceType type,
                                 struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_USER_CALL(DEV_GET_BY_TYPE, type, desc, 0);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devGetParam(const size_t deviceId,
                                const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                size_t * const result) {
    CHECKPTR(paramDescriptor);
    CHECKPTR(result);

    SYSCALL_USER_CALL(DEV_GET_PARAM, deviceId, paramDescriptor, result);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devSetParam(const size_t deviceId,
                                const struct gnwDeviceParamDescriptor * const paramDescriptor,
                                const size_t value) {
    CHECKPTR(paramDescriptor);

    SYSCALL_USER_CALL(DEV_SET_PARAM, deviceId, paramDescriptor, value);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devAcquire(const uint_32 identifier) {
    SYSCALL_USER_CALL(DEV_ACQUIRE, identifier, 0, 0);

    return SYSCALL_RESULT;
}

void devRelease(const uint_32 identifier) {
    SYSCALL_USER_CALL(DEV_RELEASE, identifier, 0, 0);
}

enum gnwDeviceError devCharWrite(const uint_32 deviceId, 
                                 const char character) {
    SYSCALL_USER_CALL(DEV_CHAR_WRITE, deviceId, character, 0);
    
    return SYSCALL_RESULT;
}

enum gnwDeviceError devMemWrite(const size_t identifier,
                                const ptr_t buffer,
                                const range_addr_t * const devInputBufferRange) {
    CHECKPTR(buffer);

    SYSCALL_USER_CALL(DEV_MEM_WRITE, identifier, buffer, devInputBufferRange);

    return SYSCALL_RESULT;
}

enum gnwDeviceError devListen(const size_t identifier,
                              const gnwDeviceEventListener listener) {
    CHECKPTR(listener);

    SYSCALL_USER_CALL(DEV_LISTEN, identifier, listener, gnwDeviceEvent_decode);

    return SYSCALL_RESULT;
}

void gnwDeviceEvent_decode(const ptr_t absDataPtr, struct gnwDeviceEvent * const absEventPtr) {
    memcopy(absDataPtr, absEventPtr, sizeof(struct gnwDeviceEvent));
    absEventPtr->data = absDataPtr + sizeof(struct gnwDeviceEvent);
}

#else

void gnwDeviceEvent_encode(const struct gnwDeviceEvent * const absEventPtr, ptr_t absDataPtr) {
    memcopy(absEventPtr, absDataPtr, sizeof(struct gnwDeviceEvent));
    memcopy(absEventPtr->data, absDataPtr + sizeof(struct gnwDeviceEvent), absEventPtr->dataSizeBytes);
}

#endif // _GUNWAPI_KERNEL
