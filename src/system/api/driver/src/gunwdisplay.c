//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/uha/gunwuha_display_desc.h"
#include "../include/gunwuha.h"
#include <gunwfug.h>

void uhaGetParam_display(const struct gnwDeviceGetParamQuery * const query) {
    CHECKPTR(query);

    const enum gnwDeviceUHA_display_param param = query->param;
    size_t result;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        extern enum gnwDeviceUHA_display_format uhaSupportedFormat(const size_t index);
        result = uhaSupportedFormat(query->paramIndex);
    } break;
    case GDU_DISPLAY_PARAM_DIMENSIONS: {
        if (query->paramIndex > 1) {
            getParamReply(false, 0);
            return;
        }

        extern point_t uhaDimensionsForFormat(const enum gnwDeviceUHA_display_format format);
        const enum gnwDeviceUHA_display_format format = query->subParam;
        const point_t dimensions = uhaDimensionsForFormat(format);
        
        result = query->paramIndex ? dimensions.y : dimensions.x;
    } break;
    default:
        getParamReply(false, 0);
        return;
    }

    getParamReply(true, result);
}

void uhaSetParam_display(const struct gnwDeviceSetParamQuery * const query) {
    CHECKPTR(query);

    const enum gnwDeviceUHA_display_param param = query->param;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        const enum gnwDeviceUHA_display_format format = query->value;
        extern bool setFormat(const enum gnwDeviceUHA_display_format);
        setParamReply(setFormat(format));
    } break;
    default:
        setParamReply(true);
    }
}

#endif // _GUNWAPI_KERNEL
