//
//  gunwdrv.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#ifndef _GUNWAPI_KERNEL

#include <types.h>
#include <scl_driver.h>
#include <gunwdevtypes.h>

void drvInitReport(bool success) {
    SYSCALL_DRIVER_CALL(REPORT_INIT, success, 0, 0);
}

void drvStartReport(bool success) {
    SYSCALL_DRIVER_CALL(REPORT_START, success, 0, 0);
}

void gnwDeviceGetParamQuery_decode(const ptr_t dataPtr, struct gnwDeviceGetParamQuery * const descPtr) {
    *(descPtr) = *((struct gnwDeviceGetParamQuery *)dataPtr);
}

void gnwDeviceSetParamQuery_decode(const ptr_t dataPtr, struct gnwDeviceSetParamQuery * const descPtr) {
    *(descPtr) = *((struct gnwDeviceSetParamQuery *)dataPtr);
}

#else

#include "../_include/_gunwdrv.h"
#include <defs.h>

static bool validateDeviceUHA_system(const struct gnwDeviceUHA * const uha) {
    if (XOR(uha->system.routine.getParam, uha->system.routine.getParamDecoder)) {
        return false;
    }
    if (XOR(uha->system.routine.setParam, uha->system.routine.setParamDecoder)) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_mem(const struct gnwDeviceUHA * const uha) {

    /*
        Validate descriptor
    */

    if (!uha->mem.desc.bytesRange.offset) {
        return false;
    }
    if (!uha->mem.desc.bytesRange.sizeBytes) {
        return false;
    }

    /*
        Validate routines
    */

    if (!uha->mem.routine.write) {
        return false;
    }
    
    return true;
}

static bool validateDeviceUHA_keyboard(const struct gnwDeviceUHA * const uha) {
    return true;
}

static bool validateDeviceUHA_mouse(const struct gnwDeviceUHA * const uha) {
    return true;
}

static bool validateDeviceUHA_display(const struct gnwDeviceUHA * const uha) {
    
    /*
        Validate descriptor
    */
    if (!uha->display.desc.supportedFormatCount) {
        return false;
    }

    /*
        Validate additional routines    
    */
    if (!uha->system.routine.getParam) {
        return false;
    }
    if (!uha->system.routine.setParam) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_charIn(const struct gnwDeviceUHA * const uha) {

    /*
        Validate routines
    */
    
    if (!uha->charIn.routine.hasData) {
        return false;
    }
    if (!uha->charIn.routine.read) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_charOut(const struct gnwDeviceUHA * const uha) {

    /*
        Validate routines
    */
    
    if (!uha->charOut.routine.isReadyToWrite) {
        return false;
    }
    if (!uha->charOut.routine.write) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_storCtrl(const struct gnwDeviceUHA * const uha) {
    
    /*
        Validate descriptor
    */

    if (!uha->storCtrl.desc.driveCount) {
        return false;
    }

    /*
        Validate routines
    */
    
    if (!uha->storCtrl.routine.driveGeometry) {
        return false;
    }
    if (!uha->storCtrl.routine.drivePresent) {
        return false;
    }
    if (!uha->storCtrl.routine.read) {
        return false;
    }

    return true;
}

static bool validateDeviceUHA_fdc(const struct gnwDeviceUHA * const uha) {
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
            if (!validateDeviceUHA_ ## NAME (&descriptor->api)) { \
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

void gnwDeviceGetParamQuery_encode(const struct gnwDeviceGetParamQuery * const descPtr, ptr_t dataPtr) {
    *((struct gnwDeviceGetParamQuery *)dataPtr) = *(descPtr);
}

void gnwDeviceSetParamQuery_encode(const struct gnwDeviceSetParamQuery * const descPtr, ptr_t dataPtr) {
    *((struct gnwDeviceSetParamQuery *)dataPtr) = *(descPtr);
}

#endif // _GUNWAPI_KERNEL