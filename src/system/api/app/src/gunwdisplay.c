//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.01.2023.
//

#include "_gunwdisplay.h"

static bool dimensionsValid(const point_t dimensions) {
    return dimensions.x > 0 && dimensions.y > 0;
}

static enum gnwDeviceUHA_display_format getSupportedFormat(const size_t deviceId, const size_t index) {
    struct gnwDeviceParamDescriptor paramDesc;

    paramDesc.param = GDU_DISPLAY_PARAM_FORMAT;
    paramDesc.subParam = 0;
    paramDesc.paramIndex = index;

    enum gnwDeviceUHA_display_format format;

    const enum gnwDeviceError err = devGetParam(deviceId, &paramDesc, (size_t *)&format);
    if (err != GDE_NONE) {
        return 0;
    }

    return format;
}

static enum gnwDeviceUHA_display_format getSupportedDisplayFormat(const enum DisplayType type,
                                                                  const struct gnwDeviceUHADesc * const deviceUHA) {
    for (size_t i = 0; i < deviceUHA->display.supportedFormatCount; ++i) {
        enum gnwDeviceUHA_display_format supportedFormat = getSupportedFormat(deviceUHA->identifier, i);
        switch (type) {
        case TEXT:
            if (GDD_FMT_ISTEXT(supportedFormat)) {
                return supportedFormat;
            }
            break;
        case GRAPHICS:
            if (GDD_FMT_ISGRAP(supportedFormat)) {
                return supportedFormat;
            }
            break;
        }
    }

    return 0;
}

static point_t getDisplayDimensions(const size_t deviceId, const enum gnwDeviceUHA_display_format format) {
    struct gnwDeviceParamDescriptor paramDesc;
    point_t dimensions;

    paramDesc.param = GDU_DISPLAY_PARAM_DIMENSIONS;
    paramDesc.subParam = format;
    
    paramDesc.paramIndex = 0; {
        const enum gnwDeviceError err = devGetParam(deviceId, &paramDesc, (size_t *)&dimensions.x);
        if (err != GDE_NONE) {
            return (point_t) { -1, -1 };
        }
    }

    paramDesc.paramIndex = 1; {
        const enum gnwDeviceError err = devGetParam(deviceId, &paramDesc, (size_t *)&dimensions.y);
        if (err != GDE_NONE) {
            return (point_t) { -1, -1 };
        }
    }

    return dimensions;
}

enum gnwDeviceError setDisplayFormat(const size_t deviceId, const enum gnwDeviceUHA_display_format format) {
    struct gnwDeviceParamDescriptor paramDesc;

    paramDesc.param = GDU_DISPLAY_PARAM_FORMAT;
    
    return devSetParam(deviceId, &paramDesc, format);
}

void fillDisplayDescriptor(const size_t identifier,
                           const enum gnwDeviceUHA_display_format format,
                           const point_t dimensions,
                           struct gnwDisplayDescriptor * const desc) {
    CHECKPTR(desc)

    desc->identifier = identifier;
    desc->dimensions = dimensions;
    desc->format = format;
}

enum gnwDeviceError getDisplay(const enum DisplayType type, 
                               struct gnwDisplayDescriptor * const displayDescriptor) {
    CHECKPTR(displayDescriptor)
    
    struct gnwDeviceUHADesc deviceUHA;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_DISPLAY, &deviceUHA);

    if (e) {
        return e;
    }

    const enum gnwDeviceUHA_display_format format = getSupportedDisplayFormat(type, &deviceUHA);
    if (!format) {
        return GDE_NOT_FOUND;
    }
    const point_t dimensions = getDisplayDimensions(deviceUHA.identifier, format);
    if (!dimensionsValid(dimensions)) {
        return GDE_UNKNOWN;
    }

    fillDisplayDescriptor(deviceUHA.identifier, format, dimensions, displayDescriptor);

    return GDE_NONE;
}

enum gnwDeviceError attachToDisplay(const enum DisplayType type, 
                                    const struct gnwDisplayDescriptor * const displayDescriptor) {
    CHECKPTR(displayDescriptor)
    
    struct gnwDeviceUHADesc uha;
    enum gnwDeviceError e = devGetById(displayDescriptor->identifier, &uha);
    
    if (e) {
        return e;
    }
    if (GDD_FMT_ISTEXT(displayDescriptor->format) != (type == TEXT)) {
        return GDE_ID_INVALID;
    }

    e = devAcquire(displayDescriptor->identifier);
    if (e) {
        return e;
    }

    e = setDisplayFormat(displayDescriptor->identifier, displayDescriptor->format);
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