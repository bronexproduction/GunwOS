//
//  ega_bus.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//

#include "ega_bus.h"
#include <gunwbus.h>
#include <error/panic.h>

#define BUS_ADDR_MONOCHROME_MARK 0xB0
#define BUS_ADDR_MONOCHROME_MARK_MASK 0xF0
#define BUS_ADDR_COLOR_OFFSET 0x20

/*
    Bus register addresses that matches 0x*B* refer to monochrome display address range.
    Corresponding bytes for color display are offset by 0x020 (addresses 0x*D*)

    For more information see:
    IBM Enhanced Graphics Adapter
    page 12+
*/
static uint_16 busAddr(uint_16 baseAddr, enum gnwDeviceUHA_display_format format) {
    if (!GDD_FMT_ISGRAP(format)) {
        return baseAddr;
    }

    if ((baseAddr & BUS_ADDR_MONOCHROME_MARK_MASK) != BUS_ADDR_MONOCHROME_MARK) {
        return baseAddr;
    }
    
    return baseAddr + BUS_ADDR_COLOR_OFFSET;
}

static void busWriteLSI(const uint_16 addrAddr, 
                        const uint_16 dataAddr, 
                        const uint_8 index, 
                        const uint_8 data, 
                        const enum gnwDeviceUHA_display_format format) {
    wrb(busAddr(addrAddr, format), index);
    wrb(busAddr(dataAddr, format), data);
}

void busWriteExternal(const enum bus_reg_external reg, const uint_8 data, const enum gnwDeviceUHA_display_format format) {
    if (reg == BRE_INPUT_STATUS_0 ||
        reg == BRE_INPUT_STATUS_1) {
        /* Read-only registers */
        OOPS("Invalid driver operation");
        return;
    }

    wrb(busAddr(reg, format), data);
}

void busWriteSequencer(const enum bus_reg_sequencer_index index, const uint_8 data, const enum gnwDeviceUHA_display_format format) {
    busWriteLSI(BRS_ADDRESS, BRS_DATA, index, data, format);
}

void busWriteCRT(const enum bus_reg_crt_index index, const uint_8 data, const enum gnwDeviceUHA_display_format format) {
    if (index == BRCI_LIGHT_PEN_HIGH ||
        index == BRCI_LIGHT_PEN_LOW) {
        /* Read-only registers */
        OOPS("Invalid driver operation");
        return;
    }
    
    busWriteLSI(BRC_ADDRESS, BRC_DATA, index, data, format);
}

void busWriteGraphicsPosition(const struct bus_reg_graphics_position position) {
    wrb(BRG_GRAPHICS_1_POSITION, (position.graphics1_pos1 ? BRG_G1PR_POSITION_1 : 0) | (position.graphics1_pos0 ? BRG_G1PR_POSITION_0 : 0));
    wrb(BRG_GRAPHICS_2_POSITION, (position.graphics2_pos1 ? BRG_G2PR_POSITION_1 : 0) | (position.graphics2_pos0 ? BRG_G2PR_POSITION_0 : 0));
}

void busWriteGraphics(const enum bus_reg_graphics_index index, const uint_8 data, const enum gnwDeviceUHA_display_format format) {
    busWriteLSI(BRG_GRAPHICS_1_AND_2_ADDRESS, BRG_DATA, index, data, format);
}

void busWriteAttribute(const enum bus_reg_attr_index index, const uint_8 data, const enum gnwDeviceUHA_display_format format) {
    busWriteLSI(BRA_ADDRESS, BRA_DATA, index, data, format);
}
