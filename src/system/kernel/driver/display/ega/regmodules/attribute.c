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
        reg.palette[0x1]     = 0x01;
        reg.palette[0x2]     = memOver64K ? 0x02 : 0x00;
        reg.palette[0x3]     = memOver64K ? 0x03 : 0x00;
        reg.palette[0x4]     = 0x04;
        reg.palette[0x5]     = memOver64K ? 0x05 : 0x07;
        reg.palette[0x6]     = memOver64K ? 0x06 : 0x00;
        reg.palette[0x7]     = memOver64K ? 0x07 : 0x00;
        reg.palette[0x8]     = memOver64K ? 0x38 : 0x00;
        reg.palette[0x9]     = memOver64K ? 0x39 : 0x01;
        reg.palette[0xA]     = memOver64K ? 0x3A : 0x00;
        reg.palette[0xB]     = memOver64K ? 0x3B : 0x00;
        reg.palette[0xC]     = memOver64K ? 0x3C : 0x04;
        reg.palette[0xD]     = memOver64K ? 0x3D : 0x07;
        reg.palette[0xE]     = memOver64K ? 0x3E : 0x00;
        reg.palette[0xF]     = memOver64K ? 0x3F : 0x00;
        reg.modeControl      = memOver64K ? 0x01 : 0x0B;
        reg.colorPlaneEnable = memOver64K ? 0x0F : 0x05;
    } break;
    }

    push(&reg, mode);
}