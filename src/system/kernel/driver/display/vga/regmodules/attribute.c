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
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "attribute.h"
#include <utils.h>
#include "../vga_bus.h"
#include <error/panic.h>

#define PALETTE_COUNT 16

struct registers {
    uint_8 palette[PALETTE_COUNT];
    uint_8 modeControl;
    uint_8 overscanColor;
    uint_8 colorPlaneEnable;
    uint_8 horizontalPelPanning;
};

static void pushConfig(const struct registers * const reg, const enum modeOfOperation mode) {
    for (int i = 0; i < PALETTE_COUNT; ++i) {
        busWriteAttribute(BRAI_PALETTE_START + i, reg->palette[i], mode);
    }
    busWriteAttribute(BRAI_MODE_CONTROL, reg->modeControl, mode);
    busWriteAttribute(BRAI_OVERSCAN_COLOR, reg->overscanColor, mode);
    busWriteAttribute(BRAI_COLOR_PLANE_ENABLE, reg->colorPlaneEnable, mode);
    busWriteAttribute(BRAI_HORIZONTAL_PEL_PANNING, reg->horizontalPelPanning, mode);
}

void attributeSetMode(const enum modeOfOperation mode) {

    struct registers reg;

    reg.palette[0x0] = 0x00;
    reg.overscanColor = 0x00;
    reg.horizontalPelPanning = 0x00;

    switch (mode) {
    case VGA_OPMODE_3: {
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
        reg.modeControl      = BRA_MCR_SELECT_BACKGROUND_INTENSITY_OR_ENABLE_BLINK; /* 0x08 */
        reg.colorPlaneEnable = BIT_RANGE_ALIGNED(0x00, BRA_CPER_VIDEO_STATUS_MUX_RANGE) |
                               BIT_RANGE_ALIGNED(0x0F, BRA_CPER_ENABLE_COLOR_PLANE_RANGE); /* 0x0F */
    } break; 
    case VGA_OPMODE_4:{
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
        reg.modeControl      = BRA_MCR_GRAPHICS_MODE; /* 0x01 */
        reg.colorPlaneEnable = BIT_RANGE_ALIGNED(0x00, BRA_CPER_VIDEO_STATUS_MUX_RANGE) |
                               BIT_RANGE_ALIGNED(0x03, BRA_CPER_ENABLE_COLOR_PLANE_RANGE); /* 0x03 */
    } break;
    default: {
        OOPS("Unsupported video mode");
    } break;
    }

    pushConfig(&reg, mode);
}

void attributeEnable(const enum modeOfOperation mode) {
    #warning why do I need it
	busWriteAttributeAddr(BRA_AAR_PALETTE_ADDRESS_SOURCE, mode);
}
