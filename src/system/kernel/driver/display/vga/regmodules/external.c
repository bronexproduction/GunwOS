//
//  external.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "External Registers" - page 12
//  Section "Programming the Registers" - page 63
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "external.h"
#include "../vga_bus.h"
#include <error/panic.h>

void externalDisable(const enum modeOfOperation mode) {
    #warning verify
    busWriteExternal(BRE_MISC_OUT, 0x00, mode);
}

void externalSetMode(const enum modeOfOperation mode, uint_8 * const regContext) {
    switch (mode) {
    case VGA_OPMODE_3:
    case VGA_OPMODE_4: {
        *regContext |= BRE_MOR_PAGE_BIT_FOR_ODD_EVEN    |
                       BRE_MOR_IO_ADDRESS_SELECT; /* 0x23 (with ENABLE_RAM) */
    } break;
    default: {
        OOPS("Unsupported video mode");
    } break;
    }

    busWriteExternal(BRE_FEATURE_CTRL, 0x00, mode);
}

void externalEnable(const enum modeOfOperation mode, uint_8 regContext) {
    regContext |= BRE_MOR_ENABLE_RAM;
    regContext &= ~BRE_MOR_DISABLE_INTERNAL_VIDEO_DRIVERS;
    
    busWriteExternal(BRE_MISC_OUT, regContext, mode);
}
