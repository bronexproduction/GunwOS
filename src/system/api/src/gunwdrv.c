//
//  gunwdrv.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#include "_gunwdrv.h"

static bool validateDeviceUHA_system(const struct gnwDeviceUHA_system * const uha) {
    return true;
}

static bool validateDeviceUHA_mem(const struct gnwDeviceUHA_mem * const uha) {

    /*
        Validate descriptor
    */

    if (!uha->desc.bytesRange.offset) {
        return false;
    }
    if (!uha->desc.bytesRange.length) {
        return false;
    }
    if (!uha->desc.maxInputSizeBytes) {
        return false;
    }

    /*
        Validate routines
    */

    if (!uha->routine.write) {
        return false;
    }
    
    return true;
}

static bool validateDeviceUHA_keyboard(const struct gnwDeviceUHA_keyboard * const uha) {
    return true;
}

static bool validateDeviceUHA_mouse(const struct gnwDeviceUHA_mouse * const uha) {
    return true;
}

static bool validateDeviceUHA_display(const struct gnwDeviceUHA_display * const uha) {
    
    /*
        Validate descriptor
    */

    if (!(uha->desc.dimensions.x && uha->desc.dimensions.y)) {
        return false;
    }
    if (!GDD_FMT_ISTEXT(uha->desc.format) &&
        !GDD_FMT_ISGRAP(uha->desc.format)) {
        return false;
    }

    /*
        Validate routines
    */

    if (!uha->routine.enable) {
        return false;
    }
    
    return true;
}

static bool validateDeviceUHA_charIn(const struct gnwDeviceUHA_charIn * const uha) {

    /*
        Validate routines
    */
    
    if (!uha->routine.hasData) {
        return false;
    }
    if (!uha->routine.read) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_charOut(const struct gnwDeviceUHA_charOut * const uha) {

    /*
        Validate routines
    */
    
    if (!uha->routine.isReadyToWrite) {
        return false;
    }
    if (!uha->routine.write) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_storCtrl(const struct gnwDeviceUHA_storCtrl * const uha) {
    
    /*
        Validate descriptor
    */

    if (!uha->desc.driveCount) {
        return false;
    }

    /*
        Validate routines
    */
    
    if (!uha->routine.driveGeometry) {
        return false;
    }
    if (!uha->routine.drivePresent) {
        return false;
    }
    if (!uha->routine.read) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_fdc(const struct gnwDeviceUHA_fdc * const uha) {
    return true;
}

bool validateDeviceDescriptor(const struct gnwDeviceDescriptor * const descriptor) {
    if (!descriptor) {
        return false;
    }
    if (!descriptor->name) {
        return false;
    }

    bool typeDefined = false;

    #define VALIDATE_UHA(TYPE, NAME) \
        if (descriptor->type & DEV_TYPE_ ## TYPE ) { \
            typeDefined = true; \
            if (!validateDeviceUHA_ ## NAME (&descriptor->api.NAME)) { \
                return false; \
            } \
        }

    VALIDATE_UHA(SYSTEM, system);
    VALIDATE_UHA(MEM, mem);
    VALIDATE_UHA(KEYBOARD, keyboard);
    VALIDATE_UHA(MOUSE, mouse);
    VALIDATE_UHA(DISPLAY, display);
    VALIDATE_UHA(CHAR_IN, charIn);
    VALIDATE_UHA(CHAR_OUT, charOut);
    VALIDATE_UHA(STORAGE, storCtrl);
    VALIDATE_UHA(FDC, fdc);

    return typeDefined;
}
