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

#define VIDEO_HW_ROWS           25
#define VIDEO_HW_COLS           80
#define VIDEO_BYTES_PER_CHAR    2

static const volatile ptr_t EGA_VIDEO_TEXT_HW_MEM = (volatile ptr_t)0xb8000;
static const volatile ptr_t EGA_VIDEO_GRAPHICS_HW_MEM = (volatile ptr_t)0xa0000;

#define EGA_BUS_MODE_CONTROL_REG    0x3D8
#define EGA_BUS_COLOR_CONTROL_REG   0x3D9

#define MEM_CHAR(INDEX) (EGA_VIDEO_TEXT_HW_MEM + (INDEX * VIDEO_BYTES_PER_CHAR))
#define MEM_COLOR(INDEX) (MEM_CHAR(INDEX) + 1)

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

    wrb(EGA_BUS_MODE_CONTROL_REG, 0);

    switch(format) {
    case TEXT_H80V25C16:
    #warning TEST       
        mode = rdb(0x3cf);
        wrb(0x3cf, mode | 1);
        mode = rdb(0x3c0);
        wrb(0x3c0, mode | 1);
        // mode = EGA_MRB_VOUT_ENABLE | EGA_MRB_GRAPHICS;
        break;
    case GRAPHICS_H320V200C16:
        // mode = EGA_MRB_VOUT_ENABLE | EGA_MRB_GRAPHICS;
        break;
    default:
        return false;
    }

    wrb(EGA_BUS_MODE_CONTROL_REG, mode);

    return true;
}

static void update(const void * const buffer, const range_addr_t inputBufferRange) {
    if (!buffer) {
        OOPS("Unexpected nullptr in buffer reference");
        return;
    }

    const struct gnwDeviceUHA_display_character * const charBuffer = (struct gnwDeviceUHA_display_character *)buffer;

    for (int index = 0; index < VIDEO_HW_COLS * VIDEO_HW_ROWS; ++index) {
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
    uha.mem.desc.bytesRange.offset = (addr_t)EGA_VIDEO_GRAPHICS_HW_MEM;
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
                /* busBase */ EGA_BUS_MODE_CONTROL_REG,
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

//     for (int index = 0; index < VIDEO_HW_COLS * VIDEO_HW_ROWS; ++index) {
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
