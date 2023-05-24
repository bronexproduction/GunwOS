//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "_gunwdisplay.h"
#include <mem.h>
#include "../include/gunwfug.h"
#include "../include/gunwipc.h"
#include <dispmgr.h>

enum gnwDeviceError getDisplay(const enum gnwDisplayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor) {
    CHECKPTR(displayDescriptor)
    
    struct gnwDisplayManagerGetDisplayQuery query;
    query.type = type;
    struct gnwDisplayManagerGetDisplayResult result;

    enum gnwIpcError error = ipcSend(DISPMGR_PATH_GET,
                                     (ptr_t)&query, sizeof(struct gnwDisplayManagerGetDisplayQuery),
                                     (ptr_t)&result, sizeof(struct gnwDisplayManagerGetDisplayResult));
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }
    
    memcopy(&result.displayDescriptor, displayDescriptor, sizeof(struct gnwDisplayDescriptor));
    displayDescriptor->dimensions.x = 80;
    displayDescriptor->dimensions.y = 25;
    displayDescriptor->format = TEXT_H80V25C16;
    displayDescriptor->identifier = 0;
    return GDE_NONE;
}

enum gnwDeviceError attachToDisplay(const enum gnwDisplayType type, 
                                    const struct gnwDisplayDescriptor * const displayDescriptor) {
    CHECKPTR(displayDescriptor)
    
    struct gnwDisplayManagerAttachToDisplayQuery query;
    query.type = type;
    query.displayDescriptor = *displayDescriptor;
    struct gnwDisplayManagerAttachToDisplayResult result;

    enum gnwIpcError error = ipcSend(DISPMGR_PATH_ATTACH,
                                     (ptr_t)&query, sizeof(struct gnwDisplayManagerAttachToDisplayQuery),
                                     (ptr_t)&result, sizeof(struct gnwDisplayManagerAttachToDisplayResult));
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }

    return GDE_NONE;
}

enum gnwDeviceError pushFrame(const size_t displayId,
                              const ptr_t buffer,
                              const size_t bufferSizeBytes) {
    CHECKPTR(buffer)
    if (bufferSizeBytes > DISPMGR_PUSH_MAX_BYTES) {
        return GDE_INVALID_PARAMETER;
    }

    range_addr_t inputRange;

    inputRange.offset = 0;
    inputRange.sizeBytes = bufferSizeBytes;

    struct gnwDisplayManagerPushFrameQuery query;
    query.displayId = displayId;
    memcopy(buffer, query.frameBuffer, bufferSizeBytes);
    query.inputRange = inputRange;
    struct gnwDisplayManagerPushFrameResult result;

    enum gnwIpcError error = ipcSend(DISPMGR_PATH_PUSH,
                                     (ptr_t)&query, sizeof(struct gnwDisplayManagerPushFrameQuery),
                                     (ptr_t)&result, sizeof(struct gnwDisplayManagerPushFrameResult));
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }
    
    return GDE_NONE;
}

#endif // _GUNWAPI_KERNEL
