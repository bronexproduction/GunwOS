//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <gunwdrv.h>

static struct gnwDriverDesc desc() {
    return (struct gnwDriverDesc){ 
        nullptr,    // init
        nullptr,    // start
        nullptr,    // isr
        NULL        // IRQ
    };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    return uha;
}

struct gnwDeviceDescriptor c_drv_display_descriptor() {
    return (struct gnwDeviceDescriptor) {
        DEV_TYPE_DISPLAY,
        uha(),
        (struct gnwDeviceDriver) {
            (struct gnwDeviceIO) {
                NULL
            },
            desc()
        },
        "Default text mode display"
    }
}
