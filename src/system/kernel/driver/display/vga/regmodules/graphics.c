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
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "graphics.h"
#include "../vga_bus.h"
#include <error/panic.h>

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

static void pushConfig(const struct registers * const reg) {
    busWriteGraphics(BRGI_SET_RESET, reg->setReset);
    busWriteGraphics(BRGI_ENABLE_SET_RESET, reg->enableSetReset);
    busWriteGraphics(BRGI_COLOR_COMPARE, reg->colorCompare);
    busWriteGraphics(BRGI_DATA_ROTATE, reg->dataRotate);
    busWriteGraphics(BRGI_READ_MAP_SELECT, reg->readMapSelect);
    busWriteGraphics(BRGI_MODE_REGISTER, reg->modeRegister);
    busWriteGraphics(BRGI_MISC, reg->misc);
    busWriteGraphics(BRGI_COLOR_DONT_CARE, reg->colorDontCare);
    busWriteGraphics(BRGI_BIT_MASK, reg->bitMask);
}

void graphicsSetMode(const enum modeOfOperation mode) {

    struct registers reg;

    reg.setReset = 0x00;
    reg.enableSetReset = 0x00;
    reg.colorCompare = 0x00;
    reg.dataRotate = 0x00;
    reg.readMapSelect = 0x00;
    reg.bitMask = 0xFF;

    switch (mode) {
    case VGA_OPMODE_3: {
        reg.modeRegister  = BRG_MR_ODD_EVEN; /* 0x10 */
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN; /* 0x0E */
        reg.colorDontCare = 0x00;
    } break;
    case VGA_OPMODE_4: {
        reg.modeRegister  = BRG_MR_SHIFT_REGISTER_MODE | BRG_MR_ODD_EVEN; /* 0x30 */
        reg.misc          = BRG_MISCR_MEMORY_MAP_1 | BRG_MISCR_MEMORY_MAP_0 | BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN | BRG_MISCR_GRAPHICS_MODE; /* 0x0F */
        reg.colorDontCare = 0x00;
    } break;
    default: {
        OOPS("Unsupported video mode",);
    } break;
    }

    pushConfig(&reg);
}
