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

static const volatile ptr_t DISPLAY_BUFFER_ADDR_BASIC = (volatile ptr_t)0xb8000;
static const volatile ptr_t DISPLAY_BUFFER_ADDR_ENHANCED = (volatile ptr_t)0xa0000;

#define BUS_ADDR_MONOCHROME_MARK 0xB0
#define BUS_ADDR_MONOCHROME_MARK_MASK 0xF0
#define BUS_ADDR_COLOR_OFFSET 0x20

enum bus_reg_external {
    BRE_MISC_OUT = 0x3C2,
    BRE_FEATURE_CTRL = 0x3BA, /* MONOCHROME ADDR */
    BRE_INPUT_STATUS_0 = 0x3C2,
    BRE_INPUT_STATUS_1 = 0x3B2, /* MONOCHROME ADDR */
};

enum bus_reg_sequencer {
    BRS_ADDRESS = 0x3C4,
    BRS_DATA = 0x3C5
};

enum bus_reg_sequencer_index {
    BRSI_RESET = 0x00,
    BRSI_CLOCKING_MODE = 0x01,
    BRSI_MAP_MASK = 0x02,
    BRSI_CHAR_MAP_SELECT = 0x03,
    BRSI_MEMORY_MODE = 0x04
};

enum bus_reg_crt {
    BRC_ADDRESS = 0x3B4, /* MONOCHROME ADDR */
    BRC_DATA = 0x3B5 /* MONOCHROME ADDR */
};

enum bus_reg_crt_index {
    BRCI_HORIZONTAL_TOTAL = 0x00,
    BRCI_HORIZONTAL_DISPLAY_END = 0x01,
    BRCI_START_HORIZONTAL_BLANK = 0x02,
    BRCI_END_HORIZONTAL_BLANK = 0x03,
    BRCI_START_HORIZONTAL_RETRACE = 0x04,
    BRCI_END_HORIZONTAL_RETRACE = 0x05,
    BRCI_VERTICAL_TOTAL = 0x06,
    BRCI_OVERFLOW = 0x07,
    BRCI_PRESET_ROW_SCAN = 0x08,
    BRCI_MAX_SCAN_LINE = 0x09,
    BRCI_CURSOR_START = 0x0A,
    BRCI_CURSOR_END = 0x0B,
    BRCI_START_ADDRESS_HIGH = 0x0C,
    BRCI_START_ADDRESS_LOW = 0x0D,
    BRCI_CURSOR_LOCATION_HIGH = 0x0E,
    BRCI_CURSOR_LOCATION_LOW = 0x0F,
    BRCI_VERTICAL_RETRACE_START = 0x10,
    BRCI_LIGHT_PEN_HIGH = 0x10,
    BRCI_VERTICAL_RETRACE_END = 0x11,
    BRCI_LIGHT_PEN_LOW = 0x11,
    BRCI_VERTICAL_DISPLAY_END = 0x12,
    BRCI_OFFSET = 0x13,
    BRCI_UNDERLINE_LOCATION = 0x14,
    BRCI_START_VERTICAL_BLANK = 0x15,
    BRCI_END_VERTICAL_BLANK = 0x16,
    BRCI_MODE_CONTROL = 0x17,
    BRCI_LINE_COMPARE = 0x18
};

enum bus_reg_graphics {
    BRG_GRAPHICS_1_POSITION = 0x3CC,
    BRG_GRAPHICS_2_POSITION = 0x3CA,
    BRG_GRAPHICS_1_AND_2_ADDRESS = 0x3CE,
    BRG_DATA = 0x3CF
};

enum bus_reg_graphics_index {
    BRGI_SET_RESET = 0x00,
    BRGI_ENABLE_SET_RESET = 0x01,
    BRGI_COLOR_COMPARE = 0x02,
    BRGI_DATA_ROTATE = 0x03,
    BRGI_READ_MAP_SELECT = 0x04,
    BRGI_MODE_REGISTER = 0x05,
    BRGI_MISC = 0x06,
    BRGI_COLOR_DONT_CARE = 0x07,
    BRGI_BIT_MASK = 0x08
};

enum bus_reg_attr {
    BRA_ADDRESS = 0x3C0,
    BRA_DATA = 0x3C0
};

enum bus_reg_attr_index {
    BRAI_PALETTE_START = 0x00, /* 0x00 - 0x0F range for 16 palettes */
    BRAI_MODE_CONTROL = 0x10,
    BRAI_OVERSCAN_COLOR = 0x11,
    BRAI_COLOR_PLANE_ENABLE = 0x12,
    BRAI_HORIZONTAL_PEL_PANNING = 0x13
};

/*
    Bus register addresses that matches 0x*B* refer to monochrome display address range.
    Corresponding bytes for color display are offset by 0x020 (addresses 0x*D*)

    For more information see:
    IBM Enhanced Graphics Adapter
    page 12+
*/
static uint_16 bus_addr(uint_16 baseAddr, enum gnwDeviceUHA_display_format format) {
    if (!GDD_FMT_ISGRAP(format)) {
        return baseAddr;
    }

    if ((baseAddr & BUS_ADDR_MONOCHROME_MARK_MASK) != BUS_ADDR_MONOCHROME_MARK) {
        return baseAddr;
    }
    
    return baseAddr + BUS_ADDR_COLOR_OFFSET;
}

#warning FUNCTION TO FETCH ADDRESS FOR SELECTED OPERATION MODE (color / mono differs)



#define BYTES_PER_CHAR    2

#define MEM_CHAR(INDEX) (DISPLAY_BUFFER_ADDR_BASIC + (INDEX * BYTES_PER_CHAR))
#define MEM_COLOR(INDEX) (MEM_CHAR(INDEX) + 1)

static point_t dimensions = { -1, -1 };

// enum k_drv_display_ega_modeRegBit {
//     /*
//         Bit 5: 1 to enable blinking, 0 to disable it
        
//         In text modes, if bit 5 is 1, characters with attribute bit 7 set will blink.
//         If not, they will have high intensity background.
//         This has no effect in graphics modes.
//     */
//     EGA_MRB_BLINK       = 0x20,

//     /*
//         Bit 4: High-resolution graphics
    
//         If this bit is set, it selects 2-colour graphics (640 pixels wide)
//         rather than 4-colour (320 pixel wide).
        
//         In text mode, setting this bit has the following effects:
//             * The border is always black.
//             * The characters displayed are missing columns
//               as if the bit pattern has been ANDed with another value.
//               According to reenigne.org, the value is the equivalent bit pattern
//               from the 640x200 graphics mode.
//     */
//     EGA_MRB_HI_RES      = 0x10,

//     /*
//         Bit 3: Enable video output
    
//         If bit 3 is 0, screen output will not be shown
//         (it will be exactly as if the video RAM contained all zeroes).
        
//         The usual use of this is if you're reprogramming the CRTC registers;
//         disable video output beforehand and re-enable it after.
//     */
//     EGA_MRB_VOUT_ENABLE = 0x08,

//     /*
//         Bit 2: Black and white
    
//         If the card is displaying on a composite monitor,
//         this disables the NTSC color(sic) burst, giving black and white output.
        
//         On an RGB monitor it has no effect except in the 320x200 graphics mode,
//         when it selects a third palette (black/red/cyan/white).
//         This palette is not documented, and not all of IBM's later EGA-compatible cards support it.
//         If this bit is set to zero in 640x200 mode, you get colour composite mode.
//     */
//     EGA_MRB_MONOCHROME  = 0x04,

//     /*
//         Bit 1: Graphics mode
        
//         If this bit is set, the display RAM will be treated as bitmap graphics rather than as text.
//     */
//     EGA_MRB_GRAPHICS    = 0x02,

//     /*
//         Bit 0: High resolution
        
//         This bit should only be set in the 80-column text mode.
        
//         It changes various timing parameters (to display 160 bytes per row instead of 80);
//         the CRTC will need to be reprogrammed accordingly.
//     */
//     EGA_MRB_HI_RES_TEXT = 0x01,
// };

// enum k_drv_display_ega_colorRegBit {
//     /*
//         Bit 5: Palette
        
//         This is only used in the 320x200 graphics mode.
        
//         If it is set, the fixed colours in the palette
//         are magenta, cyan and white. If it is reset,
//         they are red, green and yellow. 
//         Bit 2 in the mode control register (if set) overrides this bit.
//     */
//     EGA_CRB_PALETTE         = 0x20,

//     /*
//         Bit 4: Bright foreground
        
//         This is only used in the 320x200 graphics mode.

//         If set, the foreground colours display in high intensity.
//     */
//     EGA_CRB_HIGH_INTENSITY  = 0x10,

//     /*
//         Bits 3-0: Border / Background / Foreground
        
//         These 4 bits select one of the 16 EGA colours
//         (bit 3 = Intensity, Bit 2 = Red, Bit 1 = Green, Bit 0 = Blue).
        
//         In text modes, this colour is used for the border (overscan).
        
//         In 320x200 graphics modes, it is used for the background and border.
        
//         In 640x200 mode, it is used for the foreground colour.
//     */
//     EGA_CRB_INTENSITY       = 0x08,
//     EGA_CRB_RED             = 0x04,
//     EGA_CRB_GREEN           = 0x02,
//     EGA_CRB_BLUE            = 0x01,
// };

enum gnwDeviceUHA_display_format uhaSupportedFormat(const size_t index) {
    switch (index) {
    case 0:
        return TEXT_H80V25C16;
    case 1:
        return GRAPHICS_H320V200C16;
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
    case GRAPHICS_H320V200C16:
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
    uint_8 mode;

    (void)mode;
    (void)bus_addr;

    // wrb(EGA_BUS_MODE_CONTROL_REG, 0);

    switch(format) {
    case TEXT_H80V25C16:
    #warning TEST       
        // mode = rdb(0x3cf);
        // wrb(0x3cf, mode | 1);
        // mode = rdb(0x3c0);
        // wrb(0x3c0, mode | 1);
        // mode = EGA_MRB_VOUT_ENABLE | EGA_MRB_GRAPHICS;
        break;
    case GRAPHICS_H320V200C16:
        // mode = EGA_MRB_VOUT_ENABLE | EGA_MRB_GRAPHICS;
        break;
    default:
        return false;
    }

    dimensions = uhaDimensionsForFormat(format);

    // wrb(EGA_BUS_MODE_CONTROL_REG, mode);

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

// static void enable() {

// }

// static void update(const struct gnwDeviceUHA_display_character * const buffer) {
//     if (!buffer) {
//         OOPS("Unexpected nullptr in buffer reference");
//         return;
//     }

//     for (int index = 0; index < dimensions.x * dimensions.y; ++index) {
//         // *MEM_CHAR(index) = buffer[index].character;
//         // *MEM_COLOR(index) = (buffer[index].charColor | buffer[index].bgColor << 4);
//     }
// }

// static struct gnwDriverConfig desc() {
//     return (struct gnwDriverConfig){ 
//         /* init */ nullptr,
//         /* start */ nullptr,
//         /* isr */ nullptr,
//         /* IRQ */ NULL
//     };
// }

// static struct gnwDeviceUHA uha() {
//     struct gnwDeviceUHA uha;

//     uha.display.desc.dimensions = (point_t){ 320, 200 };
//     uha.display.desc.format = GRAPHICS_H320V200C16;
//     uha.display.routine.enable = enable;
//     uha.mem.desc.sizeBytes = uha.display.desc.dimensions.x * 
//                              uha.display.desc.dimensions.y /
//                              4;
//     uha.mem.desc.inputSizeBytes = uha.display.desc.dimensions.x * 
//                                   uha.display.desc.dimensions.y * 
//                                   sizeof(struct gnwDeviceUHA_display_pixel);
//     uha.mem.routine.write = (void (*)(const void * const))update;

//     return uha;
// }

// struct gnwDeviceDescriptor k_drv_display_ega_graphics_descriptor() {
//     return (struct gnwDeviceDescriptor) {
//         /* type */ DEV_TYPE_DISPLAY | DEV_TYPE_MEM,
//         /* api */ uha(),
//         /* driver */ (struct gnwDeviceDriver) {
//             /* io */ (struct gnwDeviceIO) {
//                 /* busBase */ NULL
//             },
//             /* descriptor */ desc()
//         },
//         /* name */ "Default VGA graphics mode display"
//     };
// }
