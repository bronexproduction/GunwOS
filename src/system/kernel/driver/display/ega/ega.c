//
//  ega.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.01.2023.  
//

#include <defs.h>
#include <gunwdisplaydrv.h>
#include <error/panic.h>
#include <gunwbus.h>
#include "ega_bus.h"

static const volatile ptr_t DISPLAY_BUFFER_ADDR_BASIC = (volatile ptr_t)0xb8000;
static const volatile ptr_t DISPLAY_BUFFER_ADDR_ENHANCED = (volatile ptr_t)0xa0000;

#define BYTES_PER_CHAR 2

#define MEM_CHAR(INDEX) (DISPLAY_BUFFER_ADDR_BASIC + (INDEX * BYTES_PER_CHAR))
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
    case TEXT_H80V25C16: {
        /*
            Alphanumeric Modes

            The data format for alphanumeric modes on the Enhanced Graphics Adapter
            is the same as the data format on the IBM Color/Graphics Monitor Adapter
            and the IBM Monochrome Display Adapter.
            As an added function, bit three of the attribute byte
            may be redefined by the Character Map Select register
            to act as a switch between character sets.
            This gives the programmer access to 512 characters at one time.
            This function is valid only when memory has been expanded to 128K bytes or more.
            When an alphanumeric mode is selected, the BIOS transfers character patterns
            from the ROM to bit plane 2. The processor stores the character data in bit plane 0,
            and the attribute data in bit plane 1. The programmer can view bit planes 0 and 1
            as a single buffer in alphanumeric modes.
            The CRTC generates sequential addresses, and fetches one character code byte
            and one attribute byte at a time. The character code and row scan count
            address bit plane 2, which contains the character generators.
            The appropriate dot patterns are then sent to the palette in the attribute chip,
            where color is assigned according to the attribute data.
        */

        busWriteAttribute(BRAI_MODE_CONTROL, BRA_MCR_GRAPHICS_ALPHANUMERIC_MODE, GRAPHICS_H320V200C4);
    } break;
    case GRAPHICS_H320V200C4: {
        /*
            320x200 Two and Four Color Graphics (Modes 4 and 5)
    
            Addressing, mapping and data format are the same
            as the 320x200 pel mode of the Color/Graphics Monitor Adapter.
            The display buffer is configured at hex B8000.
            Bit image data is stored in bit planes 0 and 1.
        */

        #warning TEST
        /*
            External registers
        */
        /*
            Sequencer registers
        */
        /*
            CRT registers
        */
        /*
            Graphics registers
        */
        /*
            Attribute registers
        */
    } break;
    default:
        return false;
    }

    dimensions = uhaDimensionsForFormat(format);

    return true;
}

static void update(const void * const buffer, const range_addr_t inputBufferRange) {
    if (!buffer) {
        OOPS("Unexpected nullptr in buffer reference");
        return;
    }

    const struct gnwDeviceUHA_display_character * const charBuffer = (struct gnwDeviceUHA_display_character *)buffer;

    for (int index = 0; index < dimensions.x * dimensions.y; ++index) {
        *MEM_CHAR(index) = charBuffer[index].character;
        *MEM_COLOR(index) = (charBuffer[index].charColor | charBuffer[index].bgColor << 4);
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

struct gnwDeviceDescriptor k_drv_display_ega_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ 0x3B2,
            },
            /* descriptor */ desc()
        },
        /* name */ "Default EGA display"
    };
}
