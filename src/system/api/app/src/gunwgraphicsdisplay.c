//
//  gunwgraphicsdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../_include/_gunwdisplay.h"
#include <mem.h>
#include "../include/gunwfug.h"

enum gnwDeviceError getGraphicsDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(GRAPHICS, displayDescriptor);
}

static void invalidateGraphicsHandle(struct gnwGraphicsDisplayHandle * const handle) {
    CHECKPTR(handle)
    
    devRelease(handle->descriptor.identifier);
    memset(handle, 0, sizeof(struct gnwGraphicsDisplayHandle));
}

static enum gnwDeviceError pushGraphicsFrame(struct gnwGraphicsDisplayHandle * const handle,
                                             const struct gnwDeviceUHA_display_pixel * const buffer) {
    CHECKPTR(handle)

    const size_t frameSizeBytes = handle->descriptor.dimensions.x * handle->descriptor.dimensions.y * sizeof(struct gnwDeviceUHA_display_pixel);
    enum gnwDeviceError e = pushFrame(handle->descriptor.identifier, (const ptr_t)buffer, frameSizeBytes);

    if (e == GDE_HANDLE_INVALID) {
        handle->invalidate(handle);
    }

    return e;
}

enum gnwDeviceError attachToGraphicsDisplay(const struct gnwDisplayDescriptor * const displayDescriptor,
                                            struct gnwGraphicsDisplayHandle * const handle) {
    CHECKPTR(handle)
    CHECKPTR(displayDescriptor)
    
    enum gnwDeviceError e = attachToDisplay(GRAPHICS, displayDescriptor);
    
    if (e) {
        return e;
    }

    handle->descriptor = *displayDescriptor;
    handle->invalidate = invalidateGraphicsHandle;
    handle->update = pushGraphicsFrame;

    return GDE_NONE;
}

#endif // _GUNWAPI_KERNEL
