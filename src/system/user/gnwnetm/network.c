//
//  network.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#include "network.h"

#include <mem.h>
#include <proc.h>
#include <gunwdev.h>
#include <gunwfug.h>

bool network_init() {
    struct gnwDeviceUHADesc deviceUHA;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_NETWORK, &deviceUHA);
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
