//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.04.2023.  
//

#include <defs.h>
#include <driver/gunwdrv.h>
#include <error/panic.h>
#include "../common.h"

#define VIDEO_HW_ROWS           200
#define VIDEO_HW_COLS           320
#define VIDEO_BYTES_PER_CHAR    2

static void enable() {

}

static void update(const struct gnwDeviceUHA_display_character * const buffer) {
    if (!buffer) {
        OOPS("Unexpected nullptr in buffer reference");
        return;
    }

    for (int index = 0; index < VIDEO_HW_COLS * VIDEO_HW_ROWS; ++index) {
        // *MEM_CHAR(index) = buffer[index].character;
        // *MEM_COLOR(index) = (buffer[index].charColor | buffer[index].bgColor << 4);
    }
}

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ 
        /* init */ nullptr,
        /* start */ nullptr,
        /* isr */ nullptr,
        /* IRQ */ NULL
    };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.display.desc.dimensions = (point_t){ 320, 200 };
    uha.display.desc.format = GRAPHICS_H320V200C16;
    uha.display.routine.enable = enable;
    uha.mem.desc.sizeBytes = uha.display.desc.dimensions.x * 
                             uha.display.desc.dimensions.y /
                             4;
    uha.mem.desc.inputSizeBytes = uha.display.desc.dimensions.x * 
                                  uha.display.desc.dimensions.y * 
                                  sizeof(struct gnwDeviceUHA_display_pixel);
    uha.mem.routine.write = (void (*)(const void * const))update;

    return uha;
}

struct gnwDeviceDescriptor k_drv_display_cga_graphics_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ NULL
            },
            /* descriptor */ desc()
        },
        /* name */ "Default VGA graphics mode display"
    };
}
