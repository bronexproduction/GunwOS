//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <gunwdrv.h>
#include <stdgunw/defs.h>
#include "../../../../log/log.h"

static const volatile ptr_t VIDEO_HW_MEM   = (volatile ptr_t)0xb8000;
#define VIDEO_HW_ROWS           25
#define VIDEO_HW_COLS           80
#define VIDEO_BYTES_PER_CHAR    2

#define MEM_CHAR(INDEX) (VIDEO_HW_MEM + (INDEX * VIDEO_BYTES_PER_CHAR))
#define MEM_COLOR(INDEX) (MEM_CHAR(INDEX) + 1)

static void updateText(const struct gnwDeviceUHA_display_character * const buffer) {
    if (!buffer) {
        LOG_FATAL("Unexpected nullptr in buffer reference");
        return;
    }

    for (int index = 0; index < VIDEO_HW_COLS * VIDEO_HW_ROWS; ++index) {
        *MEM_CHAR(index) = buffer[index].character;
        *MEM_COLOR(index) = (buffer[index].charColor | buffer[index].bgColor << 4);
    }
}

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ 
        nullptr,    // init
        nullptr,    // start
        nullptr,    // isr
        NULL        // IRQ
    };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.display.desc.dimensions = (point_t){ 80, 25 };
    uha.display.desc.format = TEXT_H80V25C16;
    uha.display.routine.updateText = updateText;
    uha.display.routine.updateGraphics = nullptr;

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
        "Default VGA text mode display"
    };
}
