//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.05.2023.
//

#include "display.h"

#include <gunwdev.h>
#include <gunwfug.h>

bool display_init() {
    struct gnwDeviceUHADesc deviceUHA;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_DISPLAY, &deviceUHA);
    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    e = devAcquire(deviceUHA.identifier);
    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    return true;
}
