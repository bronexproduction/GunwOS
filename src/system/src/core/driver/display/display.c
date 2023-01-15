//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <gunwdrv.h>

static struct gnwDriverDesc desc() {
    return (struct gnwDriverDesc){ 
        0,  // init
        0,  // start
        0,  // isr
        0   // IRQ
    };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.system._unused = 0;

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
