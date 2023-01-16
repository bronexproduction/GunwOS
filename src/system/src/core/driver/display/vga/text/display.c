//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <gunwdrv.h>
#include <stdgunw/defs.h>

const volatile ptr_t VIDEO_HW_MEM   = (volatile ptr_t)0xb8000;

static void updateText(const struct gnwDeviceUHA_display_character * const buffer) {

}

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

    uha.system._unused = 0;
    uha.display.dimensions = (point_t){ 80, 25 };
    uha.display.format = TEXT_H80V25C16;
    uha.display.updateText = updateText;
    uha.display.updateGraphics = nullptr;

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
    };
}
