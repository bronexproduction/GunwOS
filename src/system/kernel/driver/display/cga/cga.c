//
//  display.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <defs.h>
#include <driver/gunwdrv.h>
#include <error/panic.h>
#include "../common.h"

#define VIDEO_HW_ROWS           25
#define VIDEO_HW_COLS           80
#define VIDEO_BYTES_PER_CHAR    2

#define MEM_CHAR(INDEX) (CGA_VIDEO_HW_MEM + (INDEX * VIDEO_BYTES_PER_CHAR))
#define MEM_COLOR(INDEX) (MEM_CHAR(INDEX) + 1)

static void enable() {

}

static void update(const struct gnwDeviceUHA_display_character * const buffer) {
    if (!buffer) {
        OOPS("Unexpected nullptr in buffer reference");
        return;
    }

    for (int index = 0; index < VIDEO_HW_COLS * VIDEO_HW_ROWS; ++index) {
        *MEM_CHAR(index) = buffer[index].character;
        *MEM_COLOR(index) = (buffer[index].charColor | buffer[index].bgColor << 4);
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

    uha.display.desc.dimensions = (point_t){ 80, 25 };
    uha.display.desc.format = TEXT_H80V25C16;
    uha.display.routine.enable = enable;
    uha.mem.desc.sizeBytes = uha.display.desc.dimensions.x * 
                             uha.display.desc.dimensions.y * 
                             2;
    uha.mem.desc.inputSizeBytes = uha.display.desc.dimensions.x * 
                                  uha.display.desc.dimensions.y * 
                                  sizeof(struct gnwDeviceUHA_display_character);
    uha.mem.routine.write = (void (*)(const void * const))update;

    return uha;
}

struct gnwDeviceDescriptor k_drv_display_cga_text_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ CGA_BUS_MODE_CONTROL_REG,
            },
            /* descriptor */ desc()
        },
        /* name */ "Default VGA text mode display"
    };
}
