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

static enum gnwDeviceError getDisplay(enum displayType type, 
                                      struct gnwDisplayDescriptor * const displayDescriptor) {
    if (!displayDescriptor) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    
    struct gnwDeviceUHADesc deviceDescriptor;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_DISPLAY, &deviceDescriptor);

    // select TEXT device from the list
    // which does not exist yet

    if (!e) {
        displayDescriptor->identifier = deviceDescriptor.identifier;
        displayDescriptor->dimensions = deviceDescriptor.display.dimensions;
        displayDescriptor->format = deviceDescriptor.display.format;
    }

    return e;
}

enum gnwDeviceError getTextDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(TEXT, displayDescriptor);
}

enum gnwDeviceError getGraphicsDisplay(struct gnwDisplayDescriptor * const displayDescriptor) {
    return getDisplay(GRAPHICS, displayDescriptor);
}

enum gnwDeviceError attachToTextDisplay(uint_32 displayId, struct gnwTextDisplayHandle * const handle) {
    if (!handle) {
        fug(NULLPTR);
        __builtin_unreachable();
    }

    struct gnwDeviceUHADesc desc;
    enum gnwDeviceError e = devGetById(displayId, &desc);
    
    if (e) {
        return e;
    }
    if (!GDD_FMT_ISTEXT(desc.display.format)) {
        return GDE_ID_INVALID;
    }

    e = devAcquire(displayId);
    if (e) {
        return e;
    }

#warning TO BE IMPLEMENTED
    // struct gnwTextDisplayHandle handle = attachToDisplay(displayId, TEXT, e);

    return e;
}

enum gnwDeviceError attachToGraphicsDisplay(uint_32 displayId, struct gnwGraphicsDisplayHandle * const handle) {
    if (!handle) {
        fug(NULLPTR);
        __builtin_unreachable();
    }
    
    enum gnwDeviceError e = GDE_NONE;

    #warning TO BE IMPLEMENTED
    
    return e;
}
 