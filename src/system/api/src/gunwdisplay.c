//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#include <gunwdisplay.h>
#include <gunwdev.h>
#include <gunwfug.h>

enum displayType {
    TEXT,
    GRAPHICS
};

static void fillDisplayDescriptorWithUHA(const struct gnwDeviceUHADesc * const uha,
                                         struct gnwDisplayDescriptor * const desc) {
    if (!uha) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    if (!desc) {
        fug(NULLPTR);
        __builtin_unreachable();
    }

    desc->identifier = uha->identifier;
    desc->dimensions = uha->display.dimensions;
    desc->format = uha->display.format;
}

static enum gnwDeviceError getDisplay(enum displayType type, 
                                      struct gnwDisplayDescriptor * const displayDescriptor) {
    if (!displayDescriptor) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    
    struct gnwDeviceUHADesc deviceUHA;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_DISPLAY, &deviceUHA);

    if (e) {
        return e;
    }
    
    // select TEXT device from the list
    // which does not exist yet

    fillDisplayDescriptorWithUHA(&deviceUHA, displayDescriptor);

    return GDE_NONE;
}

enum gnwDeviceError getTextDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(TEXT, displayDescriptor);
}

enum gnwDeviceError getGraphicsDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(GRAPHICS, displayDescriptor);
}

static enum gnwDeviceError attachToDisplay(const enum displayType type, 
                                           const uint_32 displayId, 
                                           struct gnwDeviceUHADesc * const uha) {
    if (!uha) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    
    enum gnwDeviceError e = devGetById(displayId, uha);
    
    if (e) {
        return e;
    }
    if (GDD_FMT_ISTEXT(uha->display.format) != (type == TEXT)) {
        return GDE_ID_INVALID;
    }

    e = devAcquire(displayId);
    if (e) {
        return e;
    }

    return GDE_NONE;
}

static enum gnwDeviceError pushTextFrame(const struct gnwTextDisplayHandle * const handle,
                                         const struct gnwDeviceUHA_display_character * const buffer) {
#warning TO BE IMPLEMENTED
    return GDE_NONE;
}

static enum gnwDeviceError pushGraphicsFrame(const struct gnwGraphicsDisplayHandle * const handle,
                                             const struct gnwDeviceUHA_display_pixel * const buffer) {
#warning TO BE IMPLEMENTED
    return GDE_NONE;
}

enum gnwDeviceError attachToTextDisplay(uint_32 displayId, struct gnwTextDisplayHandle * const handle) {
    if (!handle) {
        fug(NULLPTR);
        __builtin_unreachable();
    }

    struct gnwDeviceUHADesc uha;
    enum gnwDeviceError e = attachToDisplay(TEXT, displayId, &uha);
    
    if (e) {
        return e;
    }

    fillDisplayDescriptorWithUHA(&uha, &handle->descriptor);
    handle->update = pushTextFrame;

    return GDE_NONE;
}

enum gnwDeviceError attachToGraphicsDisplay(uint_32 displayId, struct gnwGraphicsDisplayHandle * const handle) {
    if (!handle) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    
    struct gnwDeviceUHADesc uha;
    enum gnwDeviceError e = attachToDisplay(GRAPHICS, displayId, &uha);
    
    if (e) {
        return e;
    }

    fillDisplayDescriptorWithUHA(&uha, &handle->descriptor);
    handle->update = pushGraphicsFrame;

    return GDE_NONE;
}
 