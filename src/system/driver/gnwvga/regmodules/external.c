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
#include <gunwsheit.h>

void externalDisable() {
    #warning verify
    busWriteExternal(BRE_MISC_OUT, 0x00);
}

void externalSetMode(const enum modeOfOperation mode, uint_8 * const regContext) {
    switch (mode) {
    case VGA_OPMODE_3: {
        *regContext |= BRE_MOR_CLOCK_SELECT_0        |
                       BRE_MOR_PAGE_BIT_FOR_ODD_EVEN |
                       BRE_MOR_IO_ADDRESS_SELECT; /* 0x23 (with ENABLE_RAM) */
    } break;
    case VGA_OPMODE_4: {
        // *regContext |= BRE_MOR_PAGE_BIT_FOR_ODD_EVEN    |
        //                BRE_MOR_IO_ADDRESS_SELECT; /* 0x23 (with ENABLE_RAM) */
    } break;
    default: {
        sheit(SHEIT_NOT_SUPPORTED);
        return;
    } break;
    }

    busWriteExternal(BRE_FEATURE_CTRL, 0x00);
    busWriteExternal(BRE_MISC_OUT, *regContext);
}

void externalEnable(uint_8 regContext) {
    regContext |= BRE_MOR_ENABLE_RAM;

    busWriteExternal(BRE_MISC_OUT, regContext);
}
