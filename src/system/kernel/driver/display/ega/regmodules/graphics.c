//
//  graphics.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Graphics Controller Registers" - page 45
//  Section "Programming the Registers" - page 66
//

#include "graphics.h"
#include "../ega_bus.h"

struct registers {
    uint_8 setReset;
    uint_8 enableSetReset;
    uint_8 colorCompare;
    uint_8 dataRotate;
    uint_8 readMapSelect;
    uint_8 modeRegister;
    uint_8 misc;
    uint_8 colorDontCare;
    uint_8 bitMask;
};

static void push(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteGraphics(BRGI_SET_RESET, reg->setReset, mode);
    busWriteGraphics(BRGI_ENABLE_SET_RESET, reg->enableSetReset, mode);
    busWriteGraphics(BRGI_COLOR_COMPARE, reg->colorCompare, mode);
    busWriteGraphics(BRGI_DATA_ROTATE, reg->dataRotate, mode);
    busWriteGraphics(BRGI_READ_MAP_SELECT, reg->readMapSelect, mode);
    busWriteGraphics(BRGI_MODE_REGISTER, reg->modeRegister, mode);
    busWriteGraphics(BRGI_MISC, reg->misc, mode);
    busWriteGraphics(BRGI_COLOR_DONT_CARE, reg->colorDontCare, mode);
    busWriteGraphics(BRGI_BIT_MASK, reg->bitMask, mode);
}

void graphicsSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;

    reg.setReset = 0x00;
    reg.enableSetReset = 0x00;
    reg.colorCompare = 0x00;
    reg.dataRotate = 0x00;
    reg.readMapSelect = 0x00;
    reg.bitMask = 0xFF;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1:
    case CD_OPMODE_2:
    case CD_OPMODE_3:
    case ECD_OPMODE_0:
    case ECD_OPMODE_1:
    case ECD_OPMODE_2:
    case ECD_OPMODE_3: {
        reg.modeRegister  = BRG_MR_ODD_EVEN; /* 0x10 */
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN; /* 0x0E */
        reg.colorDontCare = 0x00;
    } break;
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        reg.modeRegister  = BRG_MR_SHIFT_REGISTER_MODE | BRG_MR_ODD_EVEN; /* 0x30 */
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN | BRG_MISCR_GRAPHICS_MODE; /* 0x0F */
        reg.colorDontCare = 0x00;
    } break;
    case CD_OPMODE_6: {
        reg.modeRegister  = 0x00;
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_GRAPHICS_MODE; /* 0x0D */
        reg.colorDontCare = 0x00;
    } break;
    case CD_OPMODE_D:
    case CD_OPMODE_E: {
        reg.modeRegister  = 0x00;
        reg.misc          = BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_GRAPHICS_MODE; /* 0x05 */
        reg.colorDontCare = BRG_CDCR_COLOR_PLANE_3_DONT_CARE | BRG_CDCR_COLOR_PLANE_2_DONT_CARE | BRG_CDCR_COLOR_PLANE_1_DONT_CARE | BRG_CDCR_COLOR_PLANE_0_DONT_CARE; /* 0x0F */
    } break;
    case MD_OPMODE_7: {
        reg.modeRegister  = BRG_MR_ODD_EVEN; /* 0x10 */
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN; /* 0x0A */
        reg.colorDontCare = 0x00;
    } break;
    case MD_OPMODE_F:
    case ECD_OPMODE_10: {
        reg.modeRegister  = memOver64K ? 0x00 : BRG_MR_ODD_EVEN; /* 0x10 */
        reg.misc          = memOver64K ? 
                            BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_GRAPHICS_MODE : /* 0x05 */
                            BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN | BRG_MISCR_GRAPHICS_MODE; /* 0x07 */
        reg.colorDontCare = BRG_CDCR_COLOR_PLANE_3_DONT_CARE | BRG_CDCR_COLOR_PLANE_2_DONT_CARE | BRG_CDCR_COLOR_PLANE_1_DONT_CARE | BRG_CDCR_COLOR_PLANE_0_DONT_CARE; /* 0x0F */
    } break;
    }

    push(&reg, mode);
}