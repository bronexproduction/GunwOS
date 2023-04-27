//
//  attribute.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Attribute Controller Registers" - page 56
//  Section "Programming the Registers" - page 67
//

#include "attribute.h"
#include "../ega_bus.h"

#define PALETTE_COUNT 16

struct registers {
    uint_8 palette[PALETTE_COUNT];
    uint_8 modeControl;
    uint_8 overscanColor;
    uint_8 colorPlaneEnable;
    uint_8 horizontalPelPanning;
};

static void push(const struct registers * const reg, const enum modeOfOperation mode) {
    for (int i = 0; i < PALETTE_COUNT; ++i) {
        busWriteAttribute(BRAI_PALETTE_START + i, reg->palette[i], mode);
    }
    busWriteAttribute(BRAI_MODE_CONTROL, reg->modeControl, mode);
    busWriteAttribute(BRAI_OVERSCAN_COLOR, reg->overscanColor, mode);
    busWriteAttribute(BRAI_COLOR_PLANE_ENABLE, reg->colorPlaneEnable, mode);
    busWriteAttribute(BRAI_HORIZONTAL_PEL_PANNING, reg->horizontalPelPanning, mode);
}

void attributeSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;


// /*
//     Attribute Controller Registers

//     Palette Register Hex 00 through Hex 0F
// */
// enum bus_reg_attr_bit_pr {
//     BRA_PR_SECONDARY_RED_VIDEO = 0x20,
//     BRA_PR_SECONDARY_GREEN_INTENSITY = 0x10,
//     BRA_PR_SECONDARY_BLUE_MONO_VIDEO = 0x08,
//     BRA_PR_RED_VIDEO = 0x04,
//     BRA_PR_GREEN_VIDEO = 0x02,
//     BRA_PR_BLUE_VIDEO = 0x01
// };

// /*
//     Attribute Controller Register

//     Mode Control Register
// */
// enum bus_reg_attr_bit_mcr {
//     BRA_MCR_SELECT_BACKGROUND_INTENSITY_OR_ENABLE_BLINK = 0x08,
//     BRA_MCR_ENABLE_LINE_GRAPHICS_CHARACTER_CODES = 0x04,
//     BRA_MCR_DISPLAY_TYPE = 0x02,
//     BRA_MCR_GRAPHICS_ALPHANUMERIC_MODE = 0x01
// };

// /*
//     Attribute Controller Registers

//     Overscan Color Register
// */
// enum bus_reg_attr_bit_ocr {
//     BRA_OCR_SELECTS_SECONDARY_RED_BORDER_COLOR = 0x20,
//     BRA_OCR_SELECTS_INTENSIFIED_OR_SECONDARY_GREEN = 0x10,
//     BRA_OCR_SELECTS_SECONDARY_BLUE_BORDER_COLOR = 0x08,
//     BRA_OCR_SELECTS_RED_BORDER_COLOR = 0x04,
//     BRA_OCR_SELECTS_GREEN_BORDER_COLOR = 0x02,
//     BRA_OCR_SELECTS_BLUE_BORDER_COLOR = 0x01
// };

// /*
//     Attribute Controller Registers

//     Color Plane Enable Register
// */
// enum bus_reg_attr_bit_cper {
//     BRA_CPER_VIDEO_STATUS_MUX_RANGE = 0x30,
//     BRA_CPER_ENABLE_COLOR_PLANE_RANGE = 0x0F
// };

// /*
//     Attribute Controller Registers

//     Horizontal Pel Panning Register
// */
// enum bus_reg_attr_bit_hppr {
//     BRA_HPPR_HORIZONTAL_PEL_PANNING_RANGE = 0x0F
// };




    
    reg.palette[0x0] = 0x00;
    reg.overscanColor = 0x00;
    reg.horizontalPelPanning = 0x00;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1:
    case CD_OPMODE_2:
    case CD_OPMODE_3: {
        reg.palette[0x1]     = BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0x2]     = BRA_PR_GREEN_VIDEO; /* 0x02 */
        reg.palette[0x3]     = BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x03 */
        reg.palette[0x4]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x5]     = BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x05 */
        reg.palette[0x6]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x06 */
        reg.palette[0x7]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY; /* 0x10 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_BLUE_VIDEO; /* 0x11 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO; /* 0x12 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x13 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO; /* 0x14 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x15 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x16 */
        reg.palette[0xF]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.modeControl      = 0x08;
        reg.colorPlaneEnable = 0x0F;
    } break; 
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        reg.palette[0x1]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x13 */
        reg.palette[0x2]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x15 */
        reg.palette[0x3]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x4]     = BRA_PR_GREEN_VIDEO; /* 0x02 */
        reg.palette[0x5]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x6]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x06 */
        reg.palette[0x7]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY; /* 0x10 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_BLUE_VIDEO; /* 0x11 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO; /* 0x12 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x13 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO; /* 0x14 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x15 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x16 */
        reg.palette[0xF]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.modeControl      = 0x01;
        reg.colorPlaneEnable = 0x03;
    } break;
    case CD_OPMODE_6: {
        reg.palette[0x1]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x2]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x3]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x4]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x5]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x6]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x7]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.palette[0xF]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.modeControl      = 0x01;
        reg.colorPlaneEnable = 0x01;
    } break;
    case CD_OPMODE_D: {
        reg.palette[0x1]     = BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0x2]     = BRA_PR_GREEN_VIDEO; /* 0x02 */
        reg.palette[0x3]     = BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x03 */
        reg.palette[0x4]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x5]     = BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x05 */
        reg.palette[0x6]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x06 */
        reg.palette[0x7]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY; /* 0x10 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_BLUE_VIDEO; /* 0x11 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO; /* 0x12 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x13 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO; /* 0x14 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x15 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x16 */
        reg.palette[0xF]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.modeControl      = 0x01;
        reg.colorPlaneEnable = 0x0F;
    } break;
    case CD_OPMODE_E: {
        reg.palette[0x1]     = BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0x2]     = BRA_PR_GREEN_VIDEO; /* 0x02 */
        reg.palette[0x3]     = BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x03 */
        reg.palette[0x4]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x5]     = BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x05 */
        reg.palette[0x6]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x06 */
        reg.palette[0x7]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY; /* 0x10 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_BLUE_VIDEO; /* 0x11 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO; /* 0x12 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x13 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO; /* 0x14 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x15 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x16 */
        reg.palette[0xF]     = 0x00;
        reg.modeControl      = 0x01;
        reg.colorPlaneEnable = 0x0F;
    } break;
    case MD_OPMODE_7: {
        reg.palette[0x1]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x2]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x3]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x4]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x5]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x6]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x7]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_GREEN_INTENSITY; /* 0x10 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xB]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xC]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xE]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xF]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x17 */
        reg.modeControl      = 0x0E;
        reg.colorPlaneEnable = 0x0F;
    } break;
    case MD_OPMODE_F: {
        reg.palette[0x1]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0x2]     = 0x00;
        reg.palette[0x3]     = 0x00;
        reg.palette[0x4]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0x5]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0x6]     = 0x00;
        reg.palette[0x7]     = 0x00;
        reg.palette[0x8]     = 0x00;
        reg.palette[0x9]     = BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x08 */
        reg.palette[0xA]     = 0x00;
        reg.palette[0xB]     = 0x00;
        reg.palette[0xC]     = 0x00;
        reg.palette[0xD]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x18 */
        reg.palette[0xE]     = 0x00;
        reg.palette[0xF]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO /* 0x3F */ 
                                          : 0x00;
        reg.modeControl      = 0x0B;
        reg.colorPlaneEnable = 0x05;
    } break;
    case ECD_OPMODE_0:
    case ECD_OPMODE_1:
    case ECD_OPMODE_2:
    case ECD_OPMODE_3: {
        reg.palette[0x1]     = BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0x2]     = BRA_PR_GREEN_VIDEO; /* 0x02 */
        reg.palette[0x3]     = BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x03 */
        reg.palette[0x4]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x5]     = BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x05 */
        reg.palette[0x6]     = BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_RED_VIDEO; /* 0x14 */
        reg.palette[0x7]     = BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x8]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO; /* 0x38 */
        reg.palette[0x9]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x39 */
        reg.palette[0xA]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x3A */
        reg.palette[0xB]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x3B */
        reg.palette[0xC]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO; /* 0x3C */
        reg.palette[0xD]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x3D */
        reg.palette[0xE]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO; /* 0x3E */
        reg.palette[0xF]     = BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x3F */
        reg.modeControl      = 0x08;
        reg.colorPlaneEnable = 0x0F;
    } break;
    case ECD_OPMODE_10: {
        reg.palette[0x1]     = BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0x2]     = memOver64K ? BRA_PR_GREEN_VIDEO /* 0x02 */
                                          : 0x00;
        reg.palette[0x3]     = memOver64K ? BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO /* 0x03 */
                                          : 0x00;
        reg.palette[0x4]     = BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0x5]     = memOver64K ? BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO /* 0x05 */
                                          : BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0x6]     = memOver64K ? BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO /* 0x06 */
                                          : 0x00;
        reg.palette[0x7]     = memOver64K ? BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO /* 0x07 */
                                          : 0x00;
        reg.palette[0x8]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO /* 0x38 */
                                          : 0x00;
        reg.palette[0x9]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_BLUE_VIDEO /* 0x39 */
                                          : BRA_PR_BLUE_VIDEO; /* 0x01 */
        reg.palette[0xA]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_GREEN_VIDEO /* 0x3A */
                                          : 0x00;
        reg.palette[0xB]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO /* 0x3B */
                                          : 0x00;
        reg.palette[0xC]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO /* 0x3C */
                                          : BRA_PR_RED_VIDEO; /* 0x04 */
        reg.palette[0xD]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_BLUE_VIDEO /* 0x3D */
                                          : BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO; /* 0x07 */
        reg.palette[0xE]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO /* 0x3E */
                                          : 0x00;
        reg.palette[0xF]     = memOver64K ? BRA_PR_SECONDARY_RED_VIDEO | BRA_PR_SECONDARY_GREEN_INTENSITY | BRA_PR_SECONDARY_BLUE_MONO_VIDEO | BRA_PR_RED_VIDEO | BRA_PR_GREEN_VIDEO | BRA_PR_BLUE_VIDEO /* 0x3F */
                                          : 0x00;
        reg.modeControl      = memOver64K ? 0x01 : 0x0B;
        reg.colorPlaneEnable = memOver64K ? 0x0F : 0x05;
    } break;
    }

    push(&reg, mode);
}