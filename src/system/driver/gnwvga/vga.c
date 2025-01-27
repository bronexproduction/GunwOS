//
//  vga.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <defs.h>
#include <gunwdrv.h>
#include <gunwfug.h>
#include <gunwmmio.h>

#include "opmode.h"

static const addr_t DISPLAY_BUFFER_ADDR_BASIC = 0xb8000;
static const addr_t DISPLAY_BUFFER_ADDR_ENHANCED = 0xa0000;
static volatile ptr_t DISPLAY_BUFFER_PTR_BASIC;
static volatile ptr_t DISPLAY_BUFFER_PTR_ENHANCED;

#define BYTES_PER_CHAR 2

#define MEM_CHAR(INDEX) ((byte_t *)(DISPLAY_BUFFER_PTR_BASIC + (INDEX * BYTES_PER_CHAR)))
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

static void update(const struct gnwDeviceMemWriteQuery * const queryPtr) {
    if (!queryPtr) {
        memWriteReply(false);
        fug(FUG_NULLPTR);
        return;
    }
    if (!queryPtr->buffer) {
        memWriteReply(false);
        fug(FUG_NULLPTR);
        return;
    }

    const struct gnwDeviceUHA_display_character * const charBuffer = (struct gnwDeviceUHA_display_character *)(queryPtr->buffer);

    for (int index = 0; index < dimensions.x * dimensions.y; ++index) {
        *MEM_CHAR(index) = charBuffer[index].character;
        *MEM_COLOR(index) = (charBuffer[index].charColor | charBuffer[index].bgColor << 4);
    }

    memWriteReply(true);
}

static void init() {
    enum gnwMemoryError error;
    DISPLAY_BUFFER_PTR_BASIC = mmioPlz(DISPLAY_BUFFER_ADDR_BASIC, BYTES_PER_CHAR * 80 * 25, &error);
    if (!DISPLAY_BUFFER_PTR_BASIC || error != GME_NONE) {
        drvInitReport(false);
        return;
    }
    DISPLAY_BUFFER_PTR_ENHANCED = mmioPlz(DISPLAY_BUFFER_ADDR_ENHANCED, 64 /* to be determined */, &error);
    if (!DISPLAY_BUFFER_PTR_ENHANCED || error != GME_NONE) {
        drvInitReport(false);
        return;
    }

    drvInitReport(true);
}

static void uhaGetParam_display(const struct gnwDeviceGetParamQuery * const query) {
    CHECKPTR(query);

    const enum gnwDeviceUHA_display_param param = query->param;
    size_t result;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        extern enum gnwDeviceUHA_display_format uhaSupportedFormat(const size_t index);
        result = uhaSupportedFormat(query->paramIndex);
    } break;
    case GDU_DISPLAY_PARAM_DIMENSIONS: {
        if (query->paramIndex > 1) {
            getParamReply(false, 0);
            return;
        }

        extern point_t uhaDimensionsForFormat(const enum gnwDeviceUHA_display_format format);
        const enum gnwDeviceUHA_display_format format = query->subParam;
        const point_t dimensions = uhaDimensionsForFormat(format);
        
        result = query->paramIndex ? dimensions.y : dimensions.x;
    } break;
    default:
        getParamReply(false, 0);
        return;
    }

    getParamReply(true, result);
}

static void uhaSetParam_display(const struct gnwDeviceSetParamQuery * const query) {
    CHECKPTR(query);

    const enum gnwDeviceUHA_display_param param = query->param;

    switch (param) {
    case GDU_DISPLAY_PARAM_FORMAT: {
        const enum gnwDeviceUHA_display_format format = query->value;
        extern bool setFormat(const enum gnwDeviceUHA_display_format);
        setParamReply(setFormat(format));
    } break;
    default:
        setParamReply(true);
    }
}

const struct gnwDeviceDescriptor _gnw_device_descriptor = {
    /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
    /* api */ { 
        /* system */ {
            /* desc */ { 0 },
            /* routine */ {
                /* getParam */ uhaGetParam_display,
                /* getParamDecoder */ gnwDeviceGetParamQuery_decode,
                /* setParam */ uhaSetParam_display,
                /* setParamDecoder */ gnwDeviceSetParamQuery_decode,
            }
        },
        /* mem */ { 
            /* desc */ {
                /* bytesRange */ {
                    /* offset */ (addr_t)DISPLAY_BUFFER_ADDR_ENHANCED,
                    /* sizeBytes */ KiB(96) /* memory between DISPLAY_BUFFER_ADDR_ENHANCED and DISPLAY_BUFFER_ADDR_BASIC */ + KiB(4) /* text mode buffer */
                },
                /* maxInputSizeBytes */ 320 * 200 * sizeof(struct gnwDeviceUHA_display_pixel)
                },
            /* routine */ {
                /* write */ update,
                /* writeDecoder */ gnwDeviceMemWriteQuery_decode
            }
        },
        GNW_UHA_NO_KEYBOARD,
        GNW_UHA_NO_MOUSE,
        GNW_UHA_NO_FDC,
        GNW_UHA_NO_STORCTRL,
        GNW_UHA_NO_CHAR_IN,
        GNW_UHA_NO_CHAR_OUT,
        /* display */ {
            /* desc */ {
                /* supportedFormatCount */ 2
            },
            /* routine */ { 0 }
        },
        GNW_UHA_NO_EVENT
    },
    /* driver */ {
        /* io */ {
            /* busBase */ 0x3B2,
        },
        /* descriptor */ {
            /* init */ init,
            /* start */ nullptr,
            /* isr */ nullptr,
            /* IRQ */ NULL
        }
    },
    /* name */ "Default VGA display"
};
