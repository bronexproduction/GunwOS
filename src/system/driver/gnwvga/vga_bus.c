//
//  vga_bus.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//

#include "vga_bus.h"
#include <gunwbus.h>
#include <gunwsheit.h>

static void vga_wrb(uint_16 const port, uint_8 const val) {
    wrb(port, val);
    extern void vga_sleepms();
    vga_sleepms();
}

static uint_8 busReadLSI(const uint_16 addrAddr,
                         const uint_16 dataAddr,
                         const uint_8 index) {
    vga_wrb(addrAddr, index);
    return rdb(dataAddr);
}

static void busWriteLSI(const uint_16 addrAddr, 
                        const uint_16 dataAddr, 
                        const uint_8 index, 
                        const uint_8 data) {
    vga_wrb(addrAddr, index);
    vga_wrb(dataAddr, data);
}

uint_8 busReadExternal(const enum bus_reg_external reg) {
    if (reg != BRE_INPUT_STATUS_0 &&
        reg != BRE_INPUT_STATUS_1) {
        /* Write-only registers */
        sheit(SHEIT_INVALID_OPERATION);
        return 0;
    }

    return rdb(reg);
}

uint_8 busReadCRT(const enum bus_reg_crt_index index) {
    if (index != BRCI_START_ADDRESS_HIGH &&
        index != BRCI_START_ADDRESS_LOW &&
        index != BRCI_CURSOR_LOCATION_HIGH &&
        index != BRCI_CURSOR_LOCATION_LOW &&
        index != BRCI_LIGHT_PEN_HIGH &&
        index != BRCI_LIGHT_PEN_LOW) {
        /* Write-only registers */
        sheit(SHEIT_INVALID_OPERATION);
        return 0;
    }

    return busReadLSI(BRC_ADDRESS, BRC_DATA, index);
}

void busWriteExternal(const enum bus_reg_external reg, const uint_8 data) {
    if (reg == BRE_INPUT_STATUS_1) {
        /* Read-only register addresses */
        sheit(SHEIT_INVALID_OPERATION);
        return;
    }

    vga_wrb(reg, data);
}

void busWriteSequencer(const enum bus_reg_sequencer_index index, const uint_8 data) {
    busWriteLSI(BRS_ADDRESS, BRS_DATA, index, data);
}

void busWriteCRT(const enum bus_reg_crt_index index, const uint_8 data) {
    busWriteLSI(BRC_ADDRESS, BRC_DATA, index, data);
}

void busWriteGraphicsPosition(const struct bus_reg_graphics_position position) {
    vga_wrb(BRG_GRAPHICS_1_POSITION, (position.graphics1_pos1 ? BRG_G1PR_POSITION_1 : 0) | (position.graphics1_pos0 ? BRG_G1PR_POSITION_0 : 0));
    vga_wrb(BRG_GRAPHICS_2_POSITION, (position.graphics2_pos1 ? BRG_G2PR_POSITION_1 : 0) | (position.graphics2_pos0 ? BRG_G2PR_POSITION_0 : 0));
}

void busWriteGraphics(const enum bus_reg_graphics_index index, const uint_8 data) {
    busWriteLSI(BRG_GRAPHICS_1_AND_2_ADDRESS, BRG_DATA, index, data);
}

void busWriteAttribute(const enum bus_reg_attr_index index, const uint_8 data) {
    /*
        Reading port 0x3DA will reset the attribute register flip-flop to address mode
    */
    (void)rdb(BRE_FEATURE_CTRL);
    busWriteLSI(BRA_ADDRESS, BRA_DATA, index, data);
}

void busWriteAttributeAddr(const uint_8 data) {
    /*
        Reading port 0x3DA will reset the attribute register flip-flop to address mode
    */
    (void)rdb(BRE_FEATURE_CTRL);
    vga_wrb(BRA_ADDRESS, data);
}