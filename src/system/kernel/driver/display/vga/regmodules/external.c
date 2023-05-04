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
//

#include "external.h"
#include "../vga_bus.h"

void externalDisable(const enum modeOfOperation mode) {
    #warning verify
    busWriteExternal(BRE_MISC_OUT, 0x00, mode);
}

void externalSetMode(const enum modeOfOperation mode, uint_8 * const regContext) {
    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1:
    case CD_OPMODE_2:
    case CD_OPMODE_3:
    case CD_OPMODE_4:
    case CD_OPMODE_5:
    case CD_OPMODE_6:
    case CD_OPMODE_D:
    case CD_OPMODE_E: {
        *regContext |= BRE_MOR_PAGE_BIT_FOR_ODD_EVEN    |
                       BRE_MOR_IO_ADDRESS_SELECT; /* 0x23 (with ENABLE_RAM) */
    } break;
    case MD_OPMODE_7: {
        *regContext |= BRE_MOR_VERTICAL_RETRACE_POLARITY    |
                       BRE_MOR_PAGE_BIT_FOR_ODD_EVEN        |
                       BRE_MOR_CLOCK_SELECT_0; /* 0xA6 (with ENABLE_RAM) */
    } break;
    case MD_OPMODE_F: {
        *regContext |= BRE_MOR_VERTICAL_RETRACE_POLARITY    |
                       BRE_MOR_PAGE_BIT_FOR_ODD_EVEN; /* 0xA2 (with ENABLE_RAM) */
    } break;
    case ECD_OPMODE_0:
    case ECD_OPMODE_1:
    case ECD_OPMODE_2:
    case ECD_OPMODE_3:
    case ECD_OPMODE_10: {
        *regContext |= BRE_MOR_VERTICAL_RETRACE_POLARITY    |
                       BRE_MOR_PAGE_BIT_FOR_ODD_EVEN        |
                       BRE_MOR_CLOCK_SELECT_0               |
                       BRE_MOR_IO_ADDRESS_SELECT; /* 0xA7 (with ENABLE_RAM) */
    } break;
    }

    busWriteExternal(BRE_FEATURE_CTRL, 0x00, mode);
}

void externalEnable(const enum modeOfOperation mode, uint_8 regContext) {
    regContext |= BRE_MOR_ENABLE_RAM;
    regContext &= ~BRE_MOR_DISABLE_INTERNAL_VIDEO_DRIVERS;
    
    busWriteExternal(BRE_MISC_OUT, regContext, mode);
}
