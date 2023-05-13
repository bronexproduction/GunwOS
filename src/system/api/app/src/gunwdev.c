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

    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(desc);

    SYSCALL_USER_FUNC(DEV_GET_BY_ID);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwDeviceError devGetByType(const enum gnwDeviceType type,
                                 struct gnwDeviceUHADesc * const desc) {
    CHECKPTR(desc);

    SYSCALL_PAR1(type);
    SYSCALL_PAR2(desc);

    SYSCALL_USER_FUNC(DEV_GET_BY_TYPE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwDeviceError devGetParam(const size_t deviceId,
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

enum gnwDeviceError devSetParam(const size_t deviceId,
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

enum gnwDeviceError devAcquire(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_USER_FUNC(DEV_ACQUIRE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

void devRelease(const uint_32 identifier) {
    SYSCALL_PAR1(identifier);

    SYSCALL_USER_FUNC(DEV_RELEASE);
    SYSCALL_USER_INT;
}

enum gnwDeviceError devCharWrite(const uint_32 deviceId, 
                                 const char character) {
    SYSCALL_PAR1(deviceId);
    SYSCALL_PAR2(character);

    SYSCALL_USER_FUNC(DEV_CHAR_WRITE);
    SYSCALL_USER_INT;
    
    register enum gnwDeviceError ret __asm__ ("eax");
    return ret;
}

enum gnwDeviceError devMemWrite(const size_t identifier,
                                const ptr_t buffer,
                                const range_addr_t * const devInputBufferRange) {
    CHECKPTR(buffer);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(buffer);
    SYSCALL_PAR3(devInputBufferRange);

    SYSCALL_USER_FUNC(DEV_MEM_WRITE);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwDeviceError devListen(const size_t identifier,
                              const gnwDeviceEventListener listener) {
    CHECKPTR(listener);

    SYSCALL_PAR1(identifier);
    SYSCALL_PAR2(listener);
    SYSCALL_PAR3(gnwDeviceEvent_decode);

    SYSCALL_USER_FUNC(DEV_LISTEN);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
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
