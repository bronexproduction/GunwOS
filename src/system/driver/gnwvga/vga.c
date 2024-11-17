//
//  vga.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <defs.h>
#include <gunwdisplaydrv.h>
#include <gunwfug.h>
#include <gunwmmio.h>

#include "opmode.h"

static volatile ptr_t DISPLAY_BUFFER_ADDR_BASIC;
static volatile ptr_t DISPLAY_BUFFER_ADDR_ENHANCED;

#define BYTES_PER_CHAR 2

#define MEM_CHAR(INDEX) ((byte_t *)(DISPLAY_BUFFER_ADDR_BASIC + (INDEX * BYTES_PER_CHAR)))
#define MEM_COLOR(INDEX) (MEM_CHAR(INDEX) + 1)

static point_t dimensions = { -1, -1 };

enum gnwDeviceUHA_display_format uhaSupportedFormat(const size_t index) {
    switch (index) {
    case 0:
        return TEXT_H80V25C16;
    case 1:
        return GRAPHICS_H320V200C4;
    default:
        return GDF_NONE;
    }
}

point_t uhaDimensionsForFormat(const enum gnwDeviceUHA_display_format format) {
    point_t dimensions;
    
    switch(format) {
    case TEXT_H80V25C16:
        dimensions.x = 80;
        dimensions.y = 25;
        break;
    case GRAPHICS_H320V200C4:
        dimensions.x = 320;
        dimensions.y = 200;
        break;
    default:
        dimensions.x = -1;
        dimensions.y = -1;
        break;
    }

    return dimensions;
}

bool setFormat(const enum gnwDeviceUHA_display_format format) {
    switch(format) {
    case TEXT_H80V25C16:
        setMode(VGA_OPMODE_3);
        break;
    case GRAPHICS_H320V200C4:
        setMode(VGA_OPMODE_4);
        break;
    default:
        return false;
    }
    
    dimensions = uhaDimensionsForFormat(format);

    return true;
}

static void update(const ptr_t buffer, const range_addr_t inputBufferRange) {
    if (!buffer) {
        fug(FUG_NULLPTR);
        return;
    }

    const struct gnwDeviceUHA_display_character * const charBuffer = (struct gnwDeviceUHA_display_character *)buffer;

    for (int index = 0; index < dimensions.x * dimensions.y; ++index) {
        *MEM_CHAR(index) = charBuffer[index].character;
        *MEM_COLOR(index) = (charBuffer[index].charColor | charBuffer[index].bgColor << 4);
    }
}

static bool init() {
    DISPLAY_BUFFER_ADDR_BASIC = mmioPlz(BYTES_PER_CHAR * 80 * 25, 0xb8000);
    DISPLAY_BUFFER_ADDR_ENHANCED = mmioPlz(64, 0xa0000);

    return true;
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.system.routine.getParam = uhaGetParam_display;
    uha.system.routine.setParam = uhaSetParam_display;
    uha.display.desc.supportedFormatCount = 2;
    uha.mem.desc.bytesRange.offset = (addr_t)DISPLAY_BUFFER_ADDR_ENHANCED;
    uha.mem.desc.bytesRange.sizeBytes = KiB(64 + 32 + 16);
    uha.mem.desc.maxInputSizeBytes = 320 * 
                                     200 * 
                                     sizeof(struct gnwDeviceUHA_display_pixel);
    uha.mem.routine.write = update;

    return uha;
}

struct gnwDeviceDescriptor _gnw_device_descriptor = (struct gnwDeviceDescriptor) {
    /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
    /* api */ uha(),
    /* driver */ (const struct gnwDeviceDriver) {
        /* io */ (const struct gnwDeviceIO) {
            /* busBase */ 0x3B2,
        },
        /* descriptor */ (const struct gnwDriverConfig) {
            /* init */ init,
            /* start */ nullptr,
            /* isr */ nullptr,
            /* IRQ */ NULL
        }
    },
    /* name */ "Default VGA display"
};
