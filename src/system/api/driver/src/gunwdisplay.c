//
//  gunwdisplay.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#warning commented until drivers running in separate processes
// #ifndef // _GUNWAPI_KERNEL

#include <uha/gunwuha_display_desc.h>

bool uhaGetParam_display(const size_t paramVal,
                         const size_t subParamVal,
                         const size_t paramIndex,
                         size_t * const result) {

    const enum gnwDeviceUHA_display_param param = paramVal;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        extern enum gnwDeviceUHA_display_format uhaSupportedFormat(const size_t index);
        *result = uhaSupportedFormat(paramIndex);
    } break;
    case GDU_DISPLAY_PARAM_DIMENSIONS: {
        if (paramIndex > 1) {
            return false;
        }

        extern point_t uhaDimensionsForFormat(const enum gnwDeviceUHA_display_format format);
        const enum gnwDeviceUHA_display_format format = subParamVal;
        const point_t dimensions = uhaDimensionsForFormat(format);
        
        *result = paramIndex ? dimensions.y : dimensions.x;
    } break;
    default:
        return false;
    }
    
    return true;
}

bool uhaSetParam_display(const size_t paramVal,
                         const size_t subParamVal,
                         const size_t paramIndex,
                         const size_t value) {

    const enum gnwDeviceUHA_display_param param = paramVal;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        const enum gnwDeviceUHA_display_format format = value;
        extern bool setFormat(const enum gnwDeviceUHA_display_format);
        return setFormat(format);
    } break;
    default:
        return false;
    }
}

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL
