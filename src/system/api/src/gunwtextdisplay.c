//
//  gunwtextdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#include "_gunwdisplay.h"

enum gnwDeviceError getTextDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(TEXT, displayDescriptor);
}

static void invalidateTextHandle(struct gnwTextDisplayHandle * const handle) {
    CHECKPTR(handle)
    
    devRelease(handle->descriptor.identifier);
    memset(handle, 0, sizeof(struct gnwTextDisplayHandle));
}

static enum gnwDeviceError pushTextFrame(struct gnwTextDisplayHandle * const handle,
                                         const struct gnwDeviceUHA_display_character * const buffer) {
    CHECKPTR(handle)

    const size_t frameSizeBytes = handle->descriptor.dimensions.x * handle->descriptor.dimensions.y * sizeof(struct gnwDeviceUHA_display_character);
    enum gnwDeviceError e = pushFrame(handle->descriptor.identifier, (const void * const)buffer, frameSizeBytes);

    if (e == GDE_HANDLE_INVALID) {
        handle->invalidate(handle);
    }

    return e;
}

enum gnwDeviceError attachToTextDisplay(uint_32 displayId, struct gnwTextDisplayHandle * const handle) {
    CHECKPTR(handle)

    struct gnwDeviceUHADesc uha;
    enum gnwDeviceError e = attachToDisplay(TEXT, displayId, &uha);
    
    if (e) {
        return e;
    }

    fillDisplayDescriptorWithUHA(&uha, &handle->descriptor);
    handle->invalidate = invalidateTextHandle;
    handle->update = pushTextFrame;

    return GDE_NONE;
}
