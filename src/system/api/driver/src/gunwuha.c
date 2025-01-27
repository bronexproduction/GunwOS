//
//  gunwuha.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifndef _GUNWAPI_KERNEL

#include <scl_driver.h>
#include "../_include/_gunwuha.h"

void getParamReply(bool success, size_t result) {
    SYSCALL_DRIVER_CALL(REPLY_GETPARAM, success, result, 0);
}

void setParamReply(bool success) {
    SYSCALL_DRIVER_CALL(REPLY_SETPARAM, success, 0, 0);
}

void memWriteReply(bool success) {
    SYSCALL_DRIVER_CALL(REPLY_MEMWRITE, success, 0, 0);
}

void gnwDeviceGetParamQuery_decode(const ptr_t dataPtr, struct gnwDeviceGetParamQuery * const queryPtr) {
    *(queryPtr) = *((struct gnwDeviceGetParamQuery *)dataPtr);
}

void gnwDeviceSetParamQuery_decode(const ptr_t dataPtr, struct gnwDeviceSetParamQuery * const queryPtr) {
    *(queryPtr) = *((struct gnwDeviceSetParamQuery *)dataPtr);
}

void gnwDeviceMemWriteQuery_decode(const ptr_t dataPtr, struct gnwDeviceMemWriteQuery * const queryPtr) {
    *(queryPtr) = *((struct gnwDeviceMemWriteQuery *)dataPtr);
    queryPtr->buffer = dataPtr + sizeof(struct gnwDeviceMemWriteQuery);
}

#else

#include "../include/gunwuha.h"
#include <mem.h>

struct gnwDeviceUHADesc uhaGetDesc(const size_t identifier,
                                   const enum gnwDeviceType type,
                                   const struct gnwDeviceUHA api) {
    return (struct gnwDeviceUHADesc) {
        identifier,
        type,
        api.system.desc,
        api.mem.desc,
        api.keyboard.desc,
        api.mouse.desc,
        api.fdc.desc,
        api.storCtrl.desc,
        api.charIn.desc,
        api.charOut.desc,
        api.display.desc,
        api.event.desc
    };
}

void gnwDeviceGetParamQuery_encode(const struct gnwDeviceGetParamQuery * const queryPtr, ptr_t dataPtr) {
    *((struct gnwDeviceGetParamQuery *)dataPtr) = *(queryPtr);
}

void gnwDeviceSetParamQuery_encode(const struct gnwDeviceSetParamQuery * const queryPtr, ptr_t dataPtr) {
    *((struct gnwDeviceSetParamQuery *)dataPtr) = *(queryPtr);
}

void gnwDeviceMemWriteQuery_encode(const struct gnwDeviceMemWriteQuery * const queryPtr, ptr_t dataPtr) {
    *((struct gnwDeviceMemWriteQuery *)dataPtr) = *(queryPtr);
    memcopy(queryPtr->buffer, dataPtr + sizeof(struct gnwDeviceMemWriteQuery), queryPtr->inputBufferRange.sizeBytes);
}

#endif // _GUNWAPI_KERNEL