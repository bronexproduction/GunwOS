//
//  gunwgraphicsdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#include "_gunwdisplay.h"

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
    enum gnwDeviceError e = pushFrame(handle->descriptor.identifier, (const void * const)buffer, frameSizeBytes);

    if (e == GDE_HANDLE_INVALID) {
        handle->invalidate(handle);
    }

    return e;
}

enum gnwDeviceError attachToGraphicsDisplay(uint_32 displayId, struct gnwGraphicsDisplayHandle * const handle) {
    CHECKPTR(handle)
    
    struct gnwDeviceUHADesc uha;
    enum gnwDeviceError e = attachToDisplay(GRAPHICS, displayId, &uha);
    
    if (e) {
        return e;
    }

    fillDisplayDescriptorWithUHA(&uha, &handle->descriptor);
    handle->invalidate = invalidateGraphicsHandle;
    handle->update = pushGraphicsFrame;

    return GDE_NONE;
}
 