//
//  vga_bus.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//

#include "vga_bus.h"
#include <gunwbus.h>
#include <error/panic.h>

#define BUS_ADDR_MONOCHROME_MARK 0xB0
#define BUS_ADDR_MONOCHROME_MARK_MASK 0xF0
#define BUS_ADDR_COLOR_OFFSET 0x20

#define IS_GRAPHICS_MODE(MODE) (mode & OPMODE_GRAP)

/*
    Bus register addresses that matches 0x*B* refer to monochrome display address range.
    Corresponding bytes for color display are offset by 0x020 (addresses 0x*D*)

    For more information see:
    IBM Enhanced Graphics Adapter
    page 12+
*/
static uint_16 busAddr(uint_16 baseAddr, const enum modeOfOperation mode) {
    if (!IS_GRAPHICS_MODE(mode)) {
        return baseAddr;
    }

    if ((baseAddr & BUS_ADDR_MONOCHROME_MARK_MASK) != BUS_ADDR_MONOCHROME_MARK) {
        return baseAddr;
    }
    
    return baseAddr + BUS_ADDR_COLOR_OFFSET;
}

static void vga_wrb(uint_16 const port, uint_8 const val, const enum modeOfOperation mode) {
    wrb(busAddr(port, mode), val);
    extern void vga_sleepms(const size_t);
    vga_sleepms(1);
}

static uint_8 busReadLSI(const uint_16 addrAddr,
                         const uint_16 dataAddr,
                         const uint_8 index,
                         const enum modeOfOperation mode) {
    vga_wrb(addrAddr, index, mode);
    return rdb(busAddr(dataAddr, mode));
}

static void busWriteLSI(const uint_16 addrAddr, 
                        const uint_16 dataAddr, 
                        const uint_8 index, 
                        const uint_8 data, 
                        const enum modeOfOperation mode) {
    vga_wrb(addrAddr, index, mode);
    vga_wrb(dataAddr, data, mode);
}

uint_8 busReadExternal(const enum bus_reg_external reg, const enum modeOfOperation mode) {
    if (reg != BRE_INPUT_STATUS_0 &&
        reg != BRE_INPUT_STATUS_1) {
        /* Write-only registers */
        OOPS("Invalid driver operation");
        return 0;
    }

    return rdb(busAddr(reg, mode));
}

uint_8 busReadCRT(const enum bus_reg_crt_index index, const enum modeOfOperation mode) {
    if (index != BRCI_START_ADDRESS_HIGH &&
        index != BRCI_START_ADDRESS_LOW &&
        index != BRCI_CURSOR_LOCATION_HIGH &&
        index != BRCI_CURSOR_LOCATION_LOW &&
        index != BRCI_LIGHT_PEN_HIGH &&
        index != BRCI_LIGHT_PEN_LOW) {
        /* Write-only registers */
        OOPS("Invalid driver operation");
        return 0;
    }

    return busReadLSI(BRC_ADDRESS, BRC_DATA, index, mode);
}

void busWriteExternal(const enum bus_reg_external reg, const uint_8 data, const enum modeOfOperation mode) {
    if (reg == BRE_INPUT_STATUS_1) {
        /* Read-only register addresses */
        OOPS("Invalid driver operation");
        return;
    }

    vga_wrb(reg, data, mode);
}

void busWriteSequencer(const enum bus_reg_sequencer_index index, const uint_8 data, const enum modeOfOperation mode) {
    busWriteLSI(BRS_ADDRESS, BRS_DATA, index, data, mode);
}

void busWriteCRT(const enum bus_reg_crt_index index, const uint_8 data, const enum modeOfOperation mode) {
    busWriteLSI(BRC_ADDRESS, BRC_DATA, index, data, mode);
}

void busWriteGraphicsPosition(const struct bus_reg_graphics_position position) {
    vga_wrb(BRG_GRAPHICS_1_POSITION, (position.graphics1_pos1 ? BRG_G1PR_POSITION_1 : 0) | (position.graphics1_pos0 ? BRG_G1PR_POSITION_0 : 0), VGA_OPMODE_0);
    vga_wrb(BRG_GRAPHICS_2_POSITION, (position.graphics2_pos1 ? BRG_G2PR_POSITION_1 : 0) | (position.graphics2_pos0 ? BRG_G2PR_POSITION_0 : 0), VGA_OPMODE_0);
}

void busWriteGraphics(const enum bus_reg_graphics_index index, const uint_8 data, const enum modeOfOperation mode) {
    busWriteLSI(BRG_GRAPHICS_1_AND_2_ADDRESS, BRG_DATA, index, data, mode);
}

void busWriteAttribute(const enum bus_reg_attr_index index, const uint_8 data, const enum modeOfOperation mode) {
    busWriteLSI(BRA_ADDRESS, BRA_DATA, index, data, mode);
}

void busWriteAttributeAddr(const uint_8 data, const enum modeOfOperation mode) {
    vga_wrb(BRA_ADDRESS, data, mode);
}