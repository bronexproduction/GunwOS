//
//  crt.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "CRT Controller Registers" - page 24
//  Section "Programming the Registers" - page 64, 65
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "crt.h"
#include <utils.h>
#include "../vga_bus.h"
#include <gunwsheit.h>

enum resolution {
    RES_320_200,
    RES_360_400,
    RES_640_200,
    RES_640_350,
    RES_640_480,
    RES_720_400
};

struct registers {
    uint_8 horizontalTotal;
    uint_8 horizontalDisplayEnd;
    uint_8 startHorizontalBlank;
    uint_8 endHorizontalBlank;
    uint_8 startHorizontalSync;
    uint_8 endHorizontalSync;
    uint_8 verticalTotal;
    uint_8 overflow;
    uint_8 presetRowScan;
    uint_8 maxScanLine;
    uint_8 cursorStart;
    uint_8 cursorEnd;
    uint_8 startAddressHigh;
    uint_8 startAddressLow;
    uint_8 cursorLocationHigh;
    uint_8 cursorLocationLow;
    uint_8 verticalSyncStart;
    uint_8 verticalDisplayEnd;
    uint_8 offset;
    uint_8 underlineLocation;
    uint_8 startVerticalBlank;
    uint_8 endVerticalBlank;
    uint_8 lineCompare;
};

#define OFFSET_LOW 0x14
#define OFFSET_HIGH 0x28

static void configure(const enum resolution res, struct registers * const reg) {
    switch (res) {
    case RES_320_200:
        // reg->horizontalTotal        = 0x37;
        // reg->horizontalDisplayEnd   = 0x27;
        // reg->verticalTotal          = 0x04;
        // reg->overflow               = BRC_CCOR_LINE_COMPARE_BIT_8 | BRC_CCOR_VERTICAL_TOTAL_BIT_8;
        // reg->verticalDisplayEnd     = 0xC7;
        // reg->startHorizontalBlank   = 0x2D;
        // reg->endHorizontalBlank     = SET(BRC_EHBR_LIGHT_PEN_ACCESS) |
        //                               BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
        //                               BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE);
        // reg->startHorizontalSync    = 0x30;
        // reg->endHorizontalSync      = CLEAR(BRC_EHSR_HORIZONTAL_BLANK_END_BIT_5)                    |
        //                               BIT_RANGE_ALIGNED(0x00, BRC_EHSR_HORIZONTAL_SYNC_DELAY_RANGE) |
        //                               BIT_RANGE_ALIGNED(0x14, BRC_EHSR_END_HORIZONTAL_SYNC_RANGE);
        // reg->maxScanLine            = 0x01;
        // reg->cursorStart            = 0x00;
        // reg->cursorEnd              = 0x00;
        // reg->verticalRetraceStart   = 0xE1;
        // reg->offset                 = OFFSET_LOW;
        // reg->underlineLocation      = 0x00;
        // reg->startVerticalBlank     = 0xE0;
        // reg->endVerticalBlank       = 0xF0;
        break;
    case RES_360_400:
        // reg->horizontalTotal = 0x2D;
        // reg->horizontalDisplayEnd = 0x27;
        // reg->verticalTotal = 0x6C;
        // reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
        //                 BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
        //                 BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
        //                 BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
        //                 BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        // reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_200:
        // reg->horizontalTotal = 0x70;
        // reg->horizontalDisplayEnd = 0x4F;
        // reg->verticalTotal = 0x04;
        // reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8 | BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x11 */
        // reg->verticalDisplayEnd = 0xC7;
        break;
    case RES_640_350:
        // reg->horizontalTotal = 0x5B;
        // reg->horizontalDisplayEnd = 0x4F;
        // reg->verticalTotal = 0x6C;
        // reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
        //                 BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
        //                 BRC_CCOR_VERTICAL_SYNC_START_BIT_8          |
        //                 BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
        //                 BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        // reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_480:
        // reg->horizontalTotal = 0x60;
        // reg->horizontalDisplayEnd = 0x4F;
        // reg->verticalTotal = 0x70;
        // reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
        //                 BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
        //                 BRC_CCOR_VERTICAL_SYNC_START_BIT_8          |
        //                 BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
        //                 BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        // reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_720_400:
        reg->horizontalTotal         = 0x5F;
        reg->horizontalDisplayEnd    = 0x4F;
        reg->verticalTotal           = 0xBF;
        reg->overflow                = BRC_CCOR_LINE_COMPARE_BIT_8                 |
                                       BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
                                       BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
                                       BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
                                       BRC_CCOR_VERTICAL_TOTAL_BIT_8;
        reg->verticalDisplayEnd      = 0x8F;
        reg->startHorizontalBlank    = 0x50;
        reg->endHorizontalBlank      = SET(BRC_EHBR_LIGHT_PEN_ACCESS) |
                                       BIT_RANGE_ALIGNED(0x00, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                       BIT_RANGE_ALIGNED(0x02, BRC_EHBR_END_BLANKING_RANGE);
        reg->startHorizontalSync     = 0x55;
        reg->endHorizontalSync       = SET(BRC_EHSR_HORIZONTAL_BLANK_END_BIT_5)                      |
                                       BIT_RANGE_ALIGNED(0x00, BRC_EHSR_HORIZONTAL_SYNC_DELAY_RANGE) |
                                       BIT_RANGE_ALIGNED(0x01, BRC_EHSR_END_HORIZONTAL_SYNC_RANGE);
        reg->maxScanLine             = 0x4F;
        reg->cursorStart             = SET(BRC_TCSR_TEXT_CURSOR_OFF);
        reg->cursorEnd               = 0x00;
        reg->verticalSyncStart       = 0x9C;
        reg->offset                  = OFFSET_HIGH;
        reg->underlineLocation       = 0x1F;
        reg->startVerticalBlank      = 0x96;
        reg->endVerticalBlank        = 0xB9;
        break;
    }
}

static void pushConfig(const struct registers * const reg) {
    busWriteCRT(BRCI_HORIZONTAL_TOTAL, reg->horizontalTotal);
    busWriteCRT(BRCI_HORIZONTAL_DISPLAY_END, reg->horizontalDisplayEnd);
    busWriteCRT(BRCI_START_HORIZONTAL_BLANK, reg->startHorizontalBlank);
    busWriteCRT(BRCI_END_HORIZONTAL_BLANK, reg->endHorizontalBlank);
    busWriteCRT(BRCI_START_HORIZONTAL_SYNC, reg->startHorizontalSync);
    busWriteCRT(BRCI_END_HORIZONTAL_SYNC, reg->endHorizontalSync);
    busWriteCRT(BRCI_VERTICAL_TOTAL, reg->verticalTotal);
    busWriteCRT(BRCI_OVERFLOW, reg->overflow);
    busWriteCRT(BRCI_PRESET_ROW_SCAN, reg->presetRowScan);
    busWriteCRT(BRCI_MAX_SCAN_LINE, reg->maxScanLine);
    busWriteCRT(BRCI_CURSOR_START, reg->cursorStart);
    busWriteCRT(BRCI_CURSOR_END, reg->cursorEnd);
    busWriteCRT(BRCI_START_ADDRESS_HIGH, reg->startAddressHigh);
    busWriteCRT(BRCI_START_ADDRESS_LOW, reg->startAddressLow);
    busWriteCRT(BRCI_CURSOR_LOCATION_HIGH, reg->cursorLocationHigh);
    busWriteCRT(BRCI_CURSOR_LOCATION_LOW, reg->cursorLocationLow);
    busWriteCRT(BRCI_VERTICAL_SYNC_START, reg->verticalSyncStart);
    busWriteCRT(BRCI_VERTICAL_DISPLAY_END, reg->verticalDisplayEnd);
    busWriteCRT(BRCI_OFFSET, reg->offset);
    busWriteCRT(BRCI_UNDERLINE_LOCATION, reg->underlineLocation);
    busWriteCRT(BRCI_START_VERTICAL_BLANK, reg->startVerticalBlank);
    busWriteCRT(BRCI_END_VERTICAL_BLANK, reg->endVerticalBlank);
    busWriteCRT(BRCI_LINE_COMPARE, reg->lineCompare);
}

void crtDisable(uint_8 * const regContextMCR, uint_8 * const regContextVSE) {
    *regContextMCR = BRC_MCR_OUTPUT_CONTROL;
    *regContextVSE = SET(BRC_VSER_ENABLE_VERTICAL_INTERRUPT) |
                     CLEAR(BRC_VSER_PROTECT_REGISTERS);

    busWriteCRT(BRCI_MODE_CONTROL, *regContextMCR);
    busWriteCRT(BRCI_VERTICAL_SYNC_END, *regContextVSE);
}

void crtSetMode(const enum modeOfOperation mode, uint_8 * const regContextMCR, uint_8 * const regContextVSE) {

    struct registers reg;

    reg.presetRowScan = 0x00;
    reg.lineCompare = 0xFF;
    reg.startAddressHigh = 0x00;
    reg.startAddressLow = 0x00;
    reg.cursorLocationHigh = 0x00;
    reg.cursorLocationLow = 0x00;

    switch (mode) {
    case VGA_OPMODE_3: {
        configure(RES_720_400, &reg);
        *regContextVSE |= BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                          BIT_RANGE_ALIGNED(0x0E, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE);
        *regContextMCR |= BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0;
    } break; 
    case VGA_OPMODE_4: {
        configure(RES_320_200, &reg);
        *regContextVSE |= BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                          BIT_RANGE_ALIGNED(0x00, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE);
        *regContextMCR |= BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER;
    } break;
    default: {
        sheit(SHEIT_NOT_SUPPORTED);
        return;
    } break;
    }

    pushConfig(&reg);
}

void crtEnable(uint_8 regContextMCR, uint_8 regContextVSE) {
    regContextVSE |= BRC_VSER_CLEAR_VERTICAL_INTERRUPT | BRC_VSER_PROTECT_REGISTERS;
    regContextVSE &= ~BRC_VSER_ENABLE_VERTICAL_INTERRUPT;
    regContextMCR |= BRC_MCR_HARDWARE_RESET;
    regContextMCR &= ~BRC_MCR_OUTPUT_CONTROL;

    busWriteCRT(BRCI_VERTICAL_SYNC_END, regContextVSE);
    busWriteCRT(BRCI_MODE_CONTROL, regContextMCR);
}
