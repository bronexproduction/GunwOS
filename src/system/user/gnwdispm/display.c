//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.05.2023.
//

#include "display.h"
#include "session.h"

#include <proc.h>
#include <gunwdev.h>
#include <gunwfug.h>

extern struct session sessions[MAX_SESSION];

bool display_init() {
    for (size_t i = 0; i < MAX_SESSION; ++i) {
        sessions[i].procId = NONE_PROC_ID;
    }    

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
