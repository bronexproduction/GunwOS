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
#include <types.h>
#include "ega_bus.h"

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
};

static void configure(const enum resolution res, struct registers * const reg) {
    switch (res) {
    case RES_320_200: // 0,1,4,5,D
        reg->horizontalTotal = 0x37;
        reg->horizontalDisplayEnd = 0x27;
        reg->verticalTotal = 0x04;
        reg->overflow = 0x11;
        reg->verticalDisplayEnd = 0xC7;
        break;
    case RES_320_350: // 0*,1*
        reg->horizontalTotal = 0x2D;
        reg->horizontalDisplayEnd = 0x27;
        reg->verticalTotal = 0x6C;
        reg->overflow = 0x1F;
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_200: // 2,3,6,E
        reg->horizontalTotal = 0x70;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x04;
        reg->overflow = 0x11;
        reg->verticalDisplayEnd = 0xC7;
        break;
    case RES_640_350: // 2*,3*,10*
        reg->horizontalTotal = 0x5B;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x6C;
        reg->overflow = 0x1F;
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_640_350_MONO: // F
        reg->horizontalTotal = 0x60;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x70;
        reg->overflow = 0x1F;
        reg->verticalDisplayEnd = 0x5D;
        break;
    case RES_720_350_MONO: // 7
        reg->horizontalTotal = 0x60;
        reg->horizontalDisplayEnd = 0x4F;
        reg->verticalTotal = 0x70;
        reg->overflow = 0x1F;
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
    busWriteCRT(BRCI_PRESET_ROW_SCAN, 0x00, mode);
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
    busWriteCRT(BRCI_LINE_COMPARE, 0xFF, mode);
}

void crtSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = 0x37;
        reg.startHorizontalRetrace = 0x31;
        reg.endHorizontalRetrace   = 0x15;
        reg.maxScanLine            = 0x07;
        reg.cursorStart            = 0x06;
        reg.cursorEnd              = 0x07;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = 0x24;
        reg.offset                 = 0x14;
        reg.underlineLocation      = 0x08;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = 0xA3;
    } break;
    case CD_OPMODE_2:
    case CD_OPMODE_3: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x5C;
        reg.endHorizontalBlank     = 0x2F;
        reg.startHorizontalRetrace = 0x5F;
        reg.endHorizontalRetrace   = 0x07;
        reg.maxScanLine            = 0x07;
        reg.cursorStart            = 0x06;
        reg.cursorEnd              = 0x07;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = 0x24;
        reg.offset                 = 0x28;
        reg.underlineLocation      = 0x08;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = 0xA3;
    } break; 
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = 0x37;
        reg.startHorizontalRetrace = 0x30;
        reg.endHorizontalRetrace   = 0x14;
        reg.maxScanLine            = 0x01;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = 0x24;
        reg.offset                 = 0x14;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xE0;
        reg.endVerticalBlank       = 0xF0;
        reg.modeControl            = 0xA2;
    } break;
    case CD_OPMODE_6: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x59;
        reg.endHorizontalBlank     = 0x2D;
        reg.startHorizontalRetrace = 0x5E;
        reg.endHorizontalRetrace   = 0x06;
        reg.maxScanLine            = 0x01;
        reg.cursorStart            = 0x00;
        reg.cursorEnd              = 0x00;
        reg.verticalRetraceStart   = 0xE0;
        reg.verticalRetraceEnd     = 0x23;
        reg.offset                 = 0x28;
        reg.underlineLocation      = 0x00;
        reg.startVerticalBlank     = 0xDF;
        reg.endVerticalBlank       = 0xEF;
        reg.modeControl            = 0xC2;
    } break; 
    case CD_OPMODE_D: {
        configure(RES_320_200, &reg);
        reg.startHorizontalBlank   = 0x2D;
        reg.endHorizontalBlank     = 0x37;
        reg.startHorizontalRetrace = 0x30;
        reg.endHorizontalRetrace   = 0x14;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0xE1;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case CD_OPMODE_E: {
        configure(RES_640_200, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = 0x2D;
        reg.startHorizontalRetrace = 0x5E;
        reg.endHorizontalRetrace   = 0x06;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case MD_OPMODE_7: {
        configure(RES_720_350_MONO, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = 0x3A;
        reg.startHorizontalRetrace = 0x51;
        reg.endHorizontalRetrace   = 0x60;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case MD_OPMODE_F: { /* MAY DEPEND ON MEMORY SIZE */
        configure(RES_640_350_MONO, &reg);
        reg.startHorizontalBlank   = 0x56;
        reg.endHorizontalBlank     = memOver64K ? 0x3A : 0x1A;
        #warning check manual for potential error in CRT register values table
        reg.startHorizontalRetrace = 0x50;
        reg.endHorizontalRetrace   = memOver64K ? 0x60 : 0xE0;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case ECD_OPMODE_0: {
        configure(RES_320_350, &reg);
        reg.startHorizontalBlank   = 0x2B;
        reg.endHorizontalBlank     = 0x2D;
        reg.startHorizontalRetrace = 0x28;
        reg.endHorizontalRetrace   = 0x6D;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case ECD_OPMODE_1: {
        configure(RES_320_350, &reg);
        reg.startHorizontalBlank   = 0x2B;
        reg.endHorizontalBlank     = 0x1D;
        reg.startHorizontalRetrace = 0x28;
        reg.endHorizontalRetrace   = 0x6D;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case ECD_OPMODE_2: {
        configure(RES_640_350, &reg);
        reg.startHorizontalBlank   = 0x53;
        reg.endHorizontalBlank     = 0x37;
        reg.startHorizontalRetrace = 0x51;
        reg.endHorizontalRetrace   = 0x5B;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case ECD_OPMODE_3: {
        configure(RES_640_350, &reg);
        reg.startHorizontalBlank   = 0x53;
        reg.endHorizontalBlank     = 0x37;
        reg.startHorizontalRetrace = 0x51;
        reg.endHorizontalRetrace   = 0x5B;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    case ECD_OPMODE_10: { /* MAY DEPEND ON INSTALLED MEMORY */
        configure(RES_640_350, &reg);
        reg.startHorizontalBlank   = 0x53;
        reg.endHorizontalBlank     = memOver64K ? 0x37 : 0x17;
        reg.startHorizontalRetrace = memOver64K ? 0x52 : 0x50;
        reg.endHorizontalRetrace   = memOver64K ? 0x00 : 0xBA;
        reg.maxScanLine            = 0x;
        reg.cursorStart            = 0x;
        reg.cursorEnd              = 0x;
        reg.verticalRetraceStart   = 0x;
        reg.verticalRetraceEnd     = 0x;
        reg.offset                 = 0x;
        reg.underlineLocation      = 0x;
        reg.startVerticalBlank     = 0x;
        reg.endVerticalBlank       = 0x;
        reg.modeControl            = 0x;
    } break; 
    }

    push(&reg, mode);
}