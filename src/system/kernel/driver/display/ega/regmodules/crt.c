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
//

#include "crt.h"
#include <utils.h>
#include "../ega_bus.h"

enum resolution {
    RES_320_200,
    RES_320_350,
    RES_640_200,
    RES_640_350,
    RES_640_350_MONO,
    RES_720_350_MONO
};

struct registers {
    uint_8 horizontalTotal;
    uint_8 horizontalDisplayEnd;
    uint_8 startHorizontalBlank;
    uint_8 endHorizontalBlank;
    uint_8 startHorizontalRetrace;
    uint_8 endHorizontalRetrace;
    uint_8 verticalTotal;
    uint_8 overflow;
    uint_8 presetRowScan;
    uint_8 maxScanLine;
    uint_8 cursorStart;
    uint_8 cursorEnd;
    uint_8 verticalRetraceStart;
    uint_8 verticalRetraceEnd;
    uint_8 verticalDisplayEnd;
    uint_8 offset;
    uint_8 underlineLocation;
    uint_8 startVerticalBlank;
    uint_8 endVerticalBlank;
    uint_8 modeControl;
    uint_8 lineCompare;
};

#define OFFSET_LOW 0x14
#define OFFSET_HIGH 0x28

static void configure(const enum resolution res, struct registers * const reg) {
    switch (res) {
    case RES_320_200: /* 0, 1, 4, 5, D */
        reg->horizontalTotal = 0x37;
        reg->horizontalDisplayEnd = 0x27;
        reg->verticalTotal = 0x04;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8 | BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x11 */
        reg->verticalDisplayEnd = 0xC7;
        break;
    case RES_320_350: /* 0*, 1* */
        reg->horizontalTotal = 0x2D;
        reg->horizontalDisplayEnd = 0x27;
        reg->verticalTotal = 0x6C;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
                        BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
                        BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
                        BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
                        BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_200: /* 2, 3, 6, E */
        reg->horizontalTotal = 0x70;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x04;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8 | BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x11 */
        reg->verticalDisplayEnd = 0xC7;
        break;
    case RES_640_350: /* 2*, 3*, 10* */
        reg->horizontalTotal = 0x5B;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x6C;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
                        BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
                        BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
                        BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
                        BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_350_MONO: /* F */
        reg->horizontalTotal = 0x60;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x70;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
                        BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
                        BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
                        BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
                        BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_720_350_MONO: /* 7 */
        reg->horizontalTotal = 0x60;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x70;
        reg->overflow = BRC_CCOR_LINE_COMPARE_BIT_8                 |
                        BRC_CCOR_START_VERTICAL_BLANK_BIT_8         |
                        BRC_CCOR_VERTICAL_RETRACE_START_BIT_8       |
                        BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8  |
                        BRC_CCOR_VERTICAL_TOTAL_BIT_8; /* 0x1F */
        reg->verticalDisplayEnd = 0x5D;
        break;
    }
}

static void push(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteCRT(BRCI_HORIZONTAL_TOTAL, reg->horizontalTotal, mode);
    busWriteCRT(BRCI_HORIZONTAL_DISPLAY_END, reg->horizontalDisplayEnd, mode);
    busWriteCRT(BRCI_START_HORIZONTAL_BLANK, reg->startHorizontalBlank, mode);
    busWriteCRT(BRCI_END_HORIZONTAL_BLANK, reg->endHorizontalBlank, mode);
    busWriteCRT(BRCI_START_HORIZONTAL_RETRACE, reg->startHorizontalRetrace, mode);
    busWriteCRT(BRCI_END_HORIZONTAL_RETRACE, reg->endHorizontalRetrace, mode);
    busWriteCRT(BRCI_VERTICAL_TOTAL, reg->verticalTotal, mode);
    busWriteCRT(BRCI_OVERFLOW, reg->overflow, mode);
    busWriteCRT(BRCI_PRESET_ROW_SCAN, reg->presetRowScan, mode);
    busWriteCRT(BRCI_MAX_SCAN_LINE, reg->maxScanLine, mode);
    busWriteCRT(BRCI_CURSOR_START, reg->cursorStart, mode);
    busWriteCRT(BRCI_CURSOR_END, reg->cursorEnd, mode);
    busWriteCRT(BRCI_VERTICAL_RETRACE_START, reg->verticalRetraceStart, mode);
    busWriteCRT(BRCI_VERTICAL_RETRACE_END, reg->verticalRetraceEnd, mode);
    busWriteCRT(BRCI_VERTICAL_DISPLAY_END, reg->verticalDisplayEnd, mode);
    busWriteCRT(BRCI_OFFSET, reg->offset, mode);
    busWriteCRT(BRCI_UNDERLINE_LOCATION, reg->underlineLocation, mode);
    busWriteCRT(BRCI_START_VERTICAL_BLANK, reg->startVerticalBlank, mode);
    busWriteCRT(BRCI_END_VERTICAL_BLANK, reg->endVerticalBlank, mode);
    busWriteCRT(BRCI_MODE_CONTROL, reg->modeControl, mode);
    busWriteCRT(BRCI_LINE_COMPARE, reg->lineCompare, mode);
}

void crtSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;

    reg.presetRowScan = 0x00;
    reg.lineCompare = 0xFF;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE); /* 0x37 */
        reg.startHorizontalRetrace = 0x31;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x15, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x15 */
        reg.maxScanLine            = 0x07;
        reg.cursorStart            = 0x06;
        reg.cursorEnd              = BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x07, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE); /* 0x07 */
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x04, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x24 */
        reg.offset                 = OFFSET_LOW;
        reg.underlineLocation      = 0x08;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xA3 */
    } break;
    case CD_OPMODE_2:
    case CD_OPMODE_3: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x5C;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0F, BRC_EHBR_END_BLANKING_RANGE); /* 0x2F */
        reg.startHorizontalRetrace = 0x5F;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x07, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x07 */
        reg.maxScanLine            = 0x07;
        reg.cursorStart            = 0x06;
        reg.cursorEnd              = BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x07, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE); /* 0x07 */
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x04, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x24 */
        reg.offset                 = OFFSET_HIGH;
        reg.underlineLocation      = 0x08;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xA3 */
    } break; 
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE); /* 0x37 */
        reg.startHorizontalRetrace = 0x30;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x14, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x14 */
        reg.maxScanLine            = 0x01;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x04, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x24 */
        reg.offset                 = OFFSET_LOW;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER; /* 0xA2 */
    } break;
    case CD_OPMODE_6: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x59;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0D, BRC_EHBR_END_BLANKING_RANGE); /* 0x2D */
        reg.startHorizontalRetrace = 0x5E;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x06, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x06 */
        reg.maxScanLine            = 0x01;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE0;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x03, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x23 */
        reg.offset                 = OFFSET_HIGH;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xDF;
        reg.endVerticalBlank       = 0xEF;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_WORD_BYTE_MODE | BRC_MCR_SELECT_ROW_SCAN_COUNTER; /* 0xC2 */
    } break; 
    case CD_OPMODE_D: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE); /* 0x37 */
        reg.startHorizontalRetrace = 0x30;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x14, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x14 */
        reg.maxScanLine            = 0x00;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x04, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x24 */
        reg.offset                 = OFFSET_LOW;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_WORD_BYTE_MODE | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xE3 */
    } break; 
    case CD_OPMODE_E: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0D, BRC_EHBR_END_BLANKING_RANGE); /* 0x2D */
        reg.startHorizontalRetrace = 0x5E;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x06, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x06 */
        reg.maxScanLine            = 0x00;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE0;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x03, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x23 */
        reg.offset                 = OFFSET_HIGH;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xDF;
        reg.endVerticalBlank       = 0xEF;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_WORD_BYTE_MODE | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xE3 */
    } break; 
    case MD_OPMODE_7: {
        configure(RES_720_350_MONO, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x1A, BRC_EHBR_END_BLANKING_RANGE); /* 0x3A */
        reg.startHorizontalRetrace = 0x51;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x03, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x60 */
        reg.maxScanLine            = 0x0D;
        reg.cursorStart            = 0x0B;
        reg.cursorEnd              = BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0C, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE); /* 0x0C */
        reg.verticalRetraceStart   = 0x5E;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x0E, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x2E */
        reg.offset                 = OFFSET_HIGH;
        reg.underlineLocation      = 0x0D;
        reg.startVerticalBlank     = 0x5E;
        reg.endVerticalBlank       = 0x6E;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xA3 */
    } break; 
    case MD_OPMODE_F: {
        configure(RES_640_350_MONO, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(memOver64K ? 0x01 : 0x00, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x1A, BRC_EHBR_END_BLANKING_RANGE); /* 0x3A or 0x1A */
        #warning should not startHorizontalRetrace be as it is in ECD_OPMODE_10 ?
        reg.startHorizontalRetrace = 0x50;
        reg.endHorizontalRetrace   = memOver64K ? CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS) : SET(BRC_EHRR_START_ODD_MEMORY_ADDRESS) |
                                     BIT_RANGE_ALIGNED(0x03, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x00, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x60 or 0xE0 */
        reg.maxScanLine            = 0x00;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0x5E;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x0E, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x2E */
        reg.offset                 = memOver64K ? OFFSET_HIGH : OFFSET_LOW;
        reg.underlineLocation      = 0x0D;
        reg.startVerticalBlank     = 0x5E;
        reg.endVerticalBlank       = 0x6E;
        reg.modeControl            = memOver64K ? BRC_MCR_HARDWARE_RESET | BRC_MCR_WORD_BYTE_MODE | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0 /* 0xE3 */ 
                                                : BRC_MCR_HARDWARE_RESET | BRC_MCR_COUNT_BY_TWO | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0x8B */
    } break; 
    case ECD_OPMODE_0:
    case ECD_OPMODE_1: {
        configure(RES_320_350, &reg);
        reg.startHorizontalBlank   = 0x2B;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0D, BRC_EHBR_END_BLANKING_RANGE); /* 0x2D */
        reg.startHorizontalRetrace = 0x28;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x03, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0D, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x6D */
        reg.maxScanLine            = 0x0D;
        reg.cursorStart            = 0x0B;
        reg.cursorEnd              = BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0C, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE); /* 0x0C */
        reg.verticalRetraceStart   = 0x5E;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x0B, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x2B */
        reg.offset                 = OFFSET_LOW;
        reg.underlineLocation      = 0x0F;
        reg.startVerticalBlank     = 0x5E;
        reg.endVerticalBlank       = 0x0A;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xA3 */
    } break; 
    case ECD_OPMODE_2:
    case ECD_OPMODE_3: {
        configure(RES_640_350, &reg);
        reg.startHorizontalBlank   = 0x53;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(0x01, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE); /* 0x37 */
        reg.startHorizontalRetrace = 0x51;
        reg.endHorizontalRetrace   = CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                     BIT_RANGE_ALIGNED(0x02, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                     BIT_RANGE_ALIGNED(0x1B, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0x5B */
        reg.maxScanLine            = 0x0D;
        reg.cursorStart            = 0x0B;
        reg.cursorEnd              = BIT_RANGE_ALIGNED(0x00, BRC_CER_CURSOR_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x0C, BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE); /* 0x0C */
        reg.verticalRetraceStart   = 0x5E;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x0B, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x2B */
        reg.offset                 = OFFSET_HIGH;
        reg.underlineLocation      = 0x0F;
        reg.startVerticalBlank     = 0x5E;
        reg.endVerticalBlank       = 0x0A;
        reg.modeControl            = BRC_MCR_HARDWARE_RESET | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0xA3 */
    } break; 
    case ECD_OPMODE_10: {
        configure(RES_640_350, &reg);
        reg.startHorizontalBlank   = 0x53;
        reg.endHorizontalBlank     = BIT_RANGE_ALIGNED(memOver64K ? 0x01 : 0x00, BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE) |
                                     BIT_RANGE_ALIGNED(0x17, BRC_EHBR_END_BLANKING_RANGE); /* 0x37 or 0x17 */
        reg.startHorizontalRetrace = memOver64K ? 0x52 : 0x50;
        reg.endHorizontalRetrace   = memOver64K ? CLEAR(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                         |
                                                  BIT_RANGE_ALIGNED(0x00, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                                  BIT_RANGE_ALIGNED(0x00, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE) /* 0x00 */
                                                : SET(BRC_EHRR_START_ODD_MEMORY_ADDRESS)                           |
                                                  BIT_RANGE_ALIGNED(0x01, BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE) |
                                                  BIT_RANGE_ALIGNED(0x1A, BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE); /* 0xBA */
        reg.maxScanLine            = 0x00;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0x5E;
        reg.verticalRetraceEnd     = SET(BRC_VRER_ENABLE_VERTICAL_INTERRUPT)  |
                                     CLEAR(BRC_VRER_CLEAR_VERTICAL_INTERRUPT) |
                                     BIT_RANGE_ALIGNED(0x0B, BRC_CER_CURSOR_SKEW_CONTROL_RANGE); /* 0x2B */
        reg.offset                 = memOver64K ? OFFSET_HIGH : OFFSET_LOW;
        reg.underlineLocation      = 0x0F;
        reg.startVerticalBlank     = 0x5F;
        reg.endVerticalBlank       = 0x0A;
        reg.modeControl            = memOver64K ? BRC_MCR_HARDWARE_RESET | BRC_MCR_WORD_BYTE_MODE | BRC_MCR_ADDRESS_WRAP | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0 /* 0xE3 */ 
                                                : BRC_MCR_HARDWARE_RESET | BRC_MCR_COUNT_BY_TWO | BRC_MCR_SELECT_ROW_SCAN_COUNTER | BRC_MCR_CMS_0; /* 0x8B */
    } break; 
    }

    push(&reg, mode);
}