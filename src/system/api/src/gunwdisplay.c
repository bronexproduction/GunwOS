//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#include "_gunwdisplay.h"

void fillDisplayDescriptorWithUHA(const struct gnwDeviceUHADesc * const uha,
                                  struct gnwDisplayDescriptor * const desc) {
    CHECKPTR(uha)
    CHECKPTR(desc)

    desc->identifier = uha->identifier;
    desc->dimensions = uha->display.dimensions;
    desc->format = uha->display.format;
}

enum gnwDeviceError getDisplay(enum DisplayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor) {
    CHECKPTR(displayDescriptor)
    
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

enum gnwDeviceError attachToDisplay(const enum DisplayType type, 
                                    const uint_32 displayId, 
                                    struct gnwDeviceUHADesc * const uha) {
    CHECKPTR(uha)
    
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

enum gnwDeviceError pushFrame(const size_t deviceId,
                              const void * const buffer,
                              const size_t bufferSizeBytes) {
    CHECKPTR(buffer)

    range_addr_t inputRange;

    inputRange.offset = 0;
    inputRange.sizeBytes = bufferSizeBytes;

    return devMemWrite(deviceId, buffer, &inputRange);
}
