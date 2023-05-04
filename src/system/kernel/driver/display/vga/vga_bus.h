//
//  vga_bus.h
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Registers"
//  page 12+
//

#ifndef VGA_BUS_H
#define VGA_BUS_H

#include <types.h>
#include "opmode.h"

enum bus_reg_external {
    BRE_MISC_OUT = 0x3C2,
    BRE_FEATURE_CTRL = 0x3BA, /* MONOCHROME ADDR */
    BRE_INPUT_STATUS_0 = 0x3C2,
    BRE_INPUT_STATUS_1 = 0x3B2, /* MONOCHROME ADDR */
};

enum bus_reg_sequencer {
    BRS_ADDRESS = 0x3C4,
    BRS_DATA = 0x3C5
};

enum bus_reg_sequencer_index {
    BRSI_RESET = 0x00,
    BRSI_CLOCKING_MODE = 0x01,
    BRSI_MAP_MASK = 0x02,
    BRSI_CHAR_MAP_SELECT = 0x03,
    BRSI_MEMORY_MODE = 0x04
};

enum bus_reg_crt {
    BRC_ADDRESS = 0x3B4, /* MONOCHROME ADDR */
    BRC_DATA = 0x3B5 /* MONOCHROME ADDR */
};

enum bus_reg_crt_index {
    BRCI_HORIZONTAL_TOTAL = 0x00,
    BRCI_HORIZONTAL_DISPLAY_END = 0x01,
    BRCI_START_HORIZONTAL_BLANK = 0x02,
    BRCI_END_HORIZONTAL_BLANK = 0x03,
    BRCI_START_HORIZONTAL_RETRACE = 0x04,
    BRCI_END_HORIZONTAL_RETRACE = 0x05,
    BRCI_VERTICAL_TOTAL = 0x06,
    BRCI_OVERFLOW = 0x07,
    BRCI_PRESET_ROW_SCAN = 0x08,
    BRCI_MAX_SCAN_LINE = 0x09,
    BRCI_CURSOR_START = 0x0A,
    BRCI_CURSOR_END = 0x0B,
    BRCI_START_ADDRESS_HIGH = 0x0C,
    BRCI_START_ADDRESS_LOW = 0x0D,
    BRCI_CURSOR_LOCATION_HIGH = 0x0E,
    BRCI_CURSOR_LOCATION_LOW = 0x0F,
    BRCI_VERTICAL_RETRACE_START = 0x10,
    BRCI_LIGHT_PEN_HIGH = 0x10,
    BRCI_VERTICAL_RETRACE_END = 0x11,
    BRCI_LIGHT_PEN_LOW = 0x11,
    BRCI_VERTICAL_DISPLAY_END = 0x12,
    BRCI_OFFSET = 0x13,
    BRCI_UNDERLINE_LOCATION = 0x14,
    BRCI_START_VERTICAL_BLANK = 0x15,
    BRCI_END_VERTICAL_BLANK = 0x16,
    BRCI_MODE_CONTROL = 0x17,
    BRCI_LINE_COMPARE = 0x18
};

enum bus_reg_graphics {
    BRG_GRAPHICS_1_POSITION = 0x3CC,
    BRG_GRAPHICS_2_POSITION = 0x3CA,
    BRG_GRAPHICS_1_AND_2_ADDRESS = 0x3CE,
    BRG_DATA = 0x3CF
};

enum bus_reg_graphics_index {
    BRGI_SET_RESET = 0x00,
    BRGI_ENABLE_SET_RESET = 0x01,
    BRGI_COLOR_COMPARE = 0x02,
    BRGI_DATA_ROTATE = 0x03,
    BRGI_READ_MAP_SELECT = 0x04,
    BRGI_MODE_REGISTER = 0x05,
    BRGI_MISC = 0x06,
    BRGI_COLOR_DONT_CARE = 0x07,
    BRGI_BIT_MASK = 0x08
};

struct bus_reg_graphics_position {
    bool graphics1_pos0 :1;
    bool graphics1_pos1 :1;
    bool graphics2_pos0 :1;
    bool graphics2_pos1 :1;
};

enum bus_reg_attr {
    BRA_ADDRESS = 0x3C0,
    BRA_DATA = 0x3C0
};

enum bus_reg_attr_index {
    BRAI_PALETTE_START = 0x00, /* 0x00 - 0x0F range for 16 palettes */
    BRAI_PALETTE_END = 0x0F,
    BRAI_MODE_CONTROL = 0x10,
    BRAI_OVERSCAN_COLOR = 0x11,
    BRAI_COLOR_PLANE_ENABLE = 0x12,
    BRAI_HORIZONTAL_PEL_PANNING = 0x13
};

/*
    External Registers

    Miscellaneous Output Register Format
*/
enum bus_reg_external_bit_mor {
    BRE_MOR_VERTICAL_RETRACE_POLARITY = 0x80,
    BRE_MOR_HORIZONTAL_RETRACE_POLARITY = 0x40,
    BRE_MOR_PAGE_BIT_FOR_ODD_EVEN = 0x20,
    BRE_MOR_DISABLE_INTERNAL_VIDEO_DRIVERS = 0x10,
    BRE_MOR_CLOCK_SELECT_1 = 0x08,
    BRE_MOR_CLOCK_SELECT_0 = 0x04,
    BRE_MOR_ENABLE_RAM = 0x02,
    BRE_MOR_IO_ADDRESS_SELECT = 0x01
};

/*
    External Registers
    
    Feature Control Register
*/
enum bus_reg_external_bit_fcr {
    BRE_FCR_FEATURE_CONTROL_1 = 0x02,
    BRE_FCR_FEATURE_CONTROL_0 = 0x01
};

/*
    External Registers
    
    Input Status Register Zero
*/
enum bus_reg_external_bit_isr0 {
    BRE_ISR0_CRT_INTERRUPT = 0x80,
    BRE_ISR0_SWITCH_SENSE = 0x10,
    BRE_ISR0_SWITCH_SENSE_RESULT_SWITCH4 = 0x08,
    BRE_ISR0_SWITCH_SENSE_RESULT_SWITCH3 = 0x04,
    BRE_ISR0_SWITCH_SENSE_RESULT_SWITCH2 = 0x02,
    BRE_ISR0_SWITCH_SENSE_RESULT_SWITCH1 = 0x01
};

/*
    External Registers
    
    Input Status Register One
*/
enum bus_reg_external_bit_isr1 {
    BRE_ISR1_DIAGNOSTIC_0 = 0x20,
    BRE_ISR1_DIAGNOSTIC_1 = 0x10,
    BRE_ISR1_VERTICAL_RETRACE = 0x08,
    BRE_ISR1_LIGHT_PEN_SWITCH = 0x04,
    BRE_ISR1_LIGHT_PEN_STROBE = 0x02,
    BRE_ISR1_DISPLAY_ENABLE = 0x01
};

/*
    Sequencer Registers

    Reset Register
*/
enum bus_reg_sequencer_bit_rr {
    BRS_RR_SYNCHRONOUS_RESET = 0x02,
    BRS_RR_ASYNCHRONOUS_RESET = 0x01
};

/*
    Sequencer Registers

    Clocking Mode Register
*/
enum bus_reg_sequencer_bit_cmr {
    BRS_CMR_DOT_CLOCK = 0x08,
    BRS_CMR_SHIFT_LOAD = 0x04,
    BRS_CMR_BANDWIDTH = 0x02,
    BRS_CMR_8_9_DOT_CLOCKS = 0x01
};

/*
    Sequencer Registers

    Map Mask Register
*/
enum bus_reg_sequencer_bit_mapMaskReg {
    BRS_MAPMASKREG_ENABLE_MAP_3 = 0x08,
    BRS_MAPMASKREG_ENABLE_MAP_2 = 0x04,
    BRS_MAPMASKREG_ENABLE_MAP_1 = 0x02,
    BRS_MAPMASKREG_ENABLE_MAP_0 = 0x01
};

/*
    Sequencer Registers

    Character Map Select Register
*/
enum bus_reg_sequencer_bit_cmsr {
    BRS_CMSR_CHARACTER_MAP_SELECT_A_RANGE = 0x0C,
    BRS_CMSR_CHARACTER_MAP_SELECT_B_RANGE = 0x03
};

/*
    Sequencer Registers

    Memory Mode Register
*/
enum bus_reg_sequencer_bit_memModeReg {
    BRS_MEMMODEREG_ODD_EVEN = 0x04,
    BRS_MEMMODEREG_EXTENDED_MEMORY = 0x02,
    BRS_MEMMODEREG_ALPHA = 0x01
};

/*
    CRT Controller Registers

    End Horizontal Blanking Register
*/
enum bus_reg_crt_bit_ehbr {
    BRC_EHBR_DISPLAY_ENABLE_SKEW_CONTROL_RANGE = 0x60,
    BRC_EHBR_END_BLANKING_RANGE = 0x1F
};

/*
    CRT Controller Registers

    End Horizontal Retrace Register
*/
enum bus_reg_crt_bit_ehrr {
    BRC_EHRR_START_ODD_MEMORY_ADDRESS = 0x80,
    BRC_EHRR_HORIZONTAL_RETRACE_DELAY_RANGE = 0x60,
    BRC_EHRR_END_HORIZONTAL_RETRACE_RANGE = 0x1F
};

/*
    CRT Controller Registers

    CRT Controller Overflow Register
*/
enum bus_reg_crt_bit_ccor {
    BRC_CCOR_CURSOR_LOCATION_BIT_8 = 0x20,
    BRC_CCOR_LINE_COMPARE_BIT_8 = 0x10,
    BRC_CCOR_START_VERTICAL_BLANK_BIT_8 = 0x08,
    BRC_CCOR_VERTICAL_RETRACE_START_BIT_8 = 0x04,
    BRC_CCOR_VERTICAL_DISPLAY_ENABLE_END_BIT_8 = 0x02,
    BRC_CCOR_VERTICAL_TOTAL_BIT_8 = 0x01
};

/*
    CRT Controller Registers

    Preset Row Scan Register
*/
enum bus_reg_crt_bit_prsr {
    BRC_PRSR_STARTING_ROW_SCAN_COUNT_AFTER_A_VERTICAL_RETRACE_RANGE = 0x1F
};

/*
    CRT Controller Registers

    Maximum Scan Line Register
*/
enum bus_reg_crt_bit_mslr {
    BRC_MSLR_MAXIMUM_SCAN_LINE_RANGE = 0x1F
};

/*
    CRT Controller Registers

    Cursor Start Register
*/
enum bus_reg_crt_bit_csr {
    BRC_CSR_ROW_SCAN_CURSOR_BEGINS_RANGE = 0x1F
};

/*
    CRT Controller Registers

    Cursor End Register
*/
enum bus_reg_crt_bit_cer {
    BRC_CER_CURSOR_SKEW_CONTROL_RANGE = 0x60,
    BRC_CER_ROW_SCAN_CURSOR_ENDS_RANGE = 0x1F
};

/*
    CRT Controller Registers

    Vertical Retrace End Register
*/
enum bus_reg_crt_bit_vrer {
    BRC_VRER_ENABLE_VERTICAL_INTERRUPT = 0x20, /* A logical 0 will enable vertical interrupt.*/
    BRC_VRER_CLEAR_VERTICAL_INTERRUPT = 0x10, /* A logical 0 will clear vertical interrupt.*/
    BRC_VRER_VERTICAL_RETRACE_END_RANGE = 0x0F
};

/*
    CRT Controller Registers

    Underline Location Register
*/
enum bus_reg_crt_bit_ulr {
    BRC_ULR_HORIZONTAL_ROW_SCAN_WHERE_UNDERLINE_WILL_OCCUR_RANGE = 0x1F
};

/*
    CRT Controller Registers

    End Vertical Blanking Register
*/
enum bus_reg_crt_bit_evbr {
    BRC_EVBR_END_VERTICAL_BLANKING_RANGE = 0x1F
};

/*
    CRT Controller Registers

    Mode Control Register
*/
enum bus_reg_crt_bit_mcr {
    BRC_MCR_HARDWARE_RESET = 0x80,
    BRC_MCR_WORD_BYTE_MODE = 0x40,
    BRC_MCR_ADDRESS_WRAP = 0x20,
    BRC_MCR_OUTPUT_CONTROL = 0x10,
    BRC_MCR_COUNT_BY_TWO = 0x08,
    BRC_MCR_HORIZONTAL_RETRACE_SELECT = 0x04,
    BRC_MCR_SELECT_ROW_SCAN_COUNTER = 0x02,
    BRC_MCR_CMS_0 = 0x01
};

/*
    Graphics Controller Registers

    Graphics 1 Position Register
*/
enum bus_reg_graphics_bit_g1pr {
    BRG_G1PR_POSITION_1 = 0x02,
    BRG_G1PR_POSITION_0 = 0x01
};

/*
    Graphics Controller Registers

    Graphics 2 Position Register
*/
enum bus_reg_graphics_bit_g2pr {
    BRG_G2PR_POSITION_1 = 0x02,
    BRG_G2PR_POSITION_0 = 0x01
};

/*
    Graphics Controller Registers

    Graphics 1 and 2 Address Register
*/
enum bus_reg_graphics_bit_g1a2ar {
    BRG_G1A2AR_GRAPHICS_ADDRESS_RANGE = 0x0F
};

/*
    Graphics Controller Registers

    Set/Reset Register
*/
enum bus_reg_graphics_bit_srr {
    BRG_SRR_SET_RESET_BIT_3 = 0x08,
    BRG_SRR_SET_RESET_BIT_2 = 0x04,
    BRG_SRR_SET_RESET_BIT_1 = 0x02,
    BRG_SRR_SET_RESET_BIT_0 = 0x01
};

/*
    Graphics Controller Registers

    Enable Set/Reset Register
*/
enum bus_reg_graphics_bit_esrr {
    BRG_ESRR_ENABLE_SET_RESET_BIT_3 = 0x08,
    BRG_ESRR_ENABLE_SET_RESET_BIT_2 = 0x04,
    BRG_ESRR_ENABLE_SET_RESET_BIT_1 = 0x02,
    BRG_ESRR_ENABLE_SET_RESET_BIT_0 = 0x01
};

/*
    Graphics Controller Registers

    Color Compare Register
*/
enum bus_reg_graphics_bit_ccr {
    BRG_CCR_COLOR_COMPARE_3 = 0x08,
    BRG_CCR_COLOR_COMPARE_2 = 0x04,
    BRG_CCR_COLOR_COMPARE_1 = 0x02,
    BRG_CCR_COLOR_COMPARE_0 = 0x01
};

/*
    Graphics Controller Registers

    Data Rotate Register
*/
enum bus_reg_graphics_bit_drr {
    BRG_DRR_FUNCTION_SELECT_RANGE = 0x18,
    BRG_DRR_ROTATE_COUNT_2 = 0x04,
    BRG_DRR_ROTATE_COUNT_1 = 0x02,
    BRG_DRR_ROTATE_COUNT = 0x01
};

/*
    Graphics Controller Registers

    Read Map Select Register
*/
enum bus_reg_graphics_bit_rmsr {
    BRG_RMSR_MAP_SELECT_2 = 0x04,
    BRG_RMSR_MAP_SELECT_1 = 0x02,
    BRG_RMSR_MAP_SELECT_0 = 0x01
};

/*
    Graphics Controller Registers

    Mode Register
*/
enum bus_reg_graphics_bit_mr {
    BRG_MR_SHIFT_REGISTER_MODE = 0x20,
    BRG_MR_ODD_EVEN = 0x10,
    BRG_MR_READ_MODE = 0x08,
    BRG_MR_TEST_CONDITION = 0x04,
    BRG_MR_WRITE_MODE_RANGE = 0x03,
};

/*
    Graphics Controller Registers

    Miscellaneous Register
*/
enum bus_reg_graphics_bit_miscr {
    BRG_MISCR_MEMORY_MAP_1 = 0x08,
    BRG_MISCR_MEMORY_MAP_0 = 0x04,
    BRG_MISCR_CHAIN_ODD_MAPS_TO_EVEN = 0x02,
    BRG_MISCR_GRAPHICS_MODE = 0x01
};

/*
    Graphics Controller Registers

    Color Don't Care Register
*/
enum bus_reg_graphics_bit_cdcr {
    BRG_CDCR_COLOR_PLANE_3_DONT_CARE = 0x08,
    BRG_CDCR_COLOR_PLANE_2_DONT_CARE = 0x04,
    BRG_CDCR_COLOR_PLANE_1_DONT_CARE = 0x02,
    BRG_CDCR_COLOR_PLANE_0_DONT_CARE = 0x01
};

/*
    Attribute Controller Registers

    Atribute Address Register
*/
enum bus_reg_attr_bit_aar {
    BRA_AAR_PALETTE_ADDRESS_SOURCE = 0x20,
    BRA_AAR_ATTRIBUTE_ADDRESS_RANGE = 0x1F
};

/*
    Attribute Controller Registers

    Palette Register Hex 00 through Hex 0F
*/
enum bus_reg_attr_bit_pr {
    BRA_PR_SECONDARY_RED_VIDEO = 0x20,
    BRA_PR_SECONDARY_GREEN_INTENSITY = 0x10,
    BRA_PR_SECONDARY_BLUE_MONO_VIDEO = 0x08,
    BRA_PR_RED_VIDEO = 0x04,
    BRA_PR_GREEN_VIDEO = 0x02,
    BRA_PR_BLUE_VIDEO = 0x01
};

/*
    Attribute Controller Register

    Mode Control Register
*/
enum bus_reg_attr_bit_mcr {
    BRA_MCR_SELECT_BACKGROUND_INTENSITY_OR_ENABLE_BLINK = 0x08,
    BRA_MCR_ENABLE_LINE_GRAPHICS_CHARACTER_CODES = 0x04,
    BRA_MCR_DISPLAY_TYPE = 0x02,
    BRA_MCR_GRAPHICS_MODE = 0x01
};

/*
    Attribute Controller Registers

    Overscan Color Register
*/
enum bus_reg_attr_bit_ocr {
    BRA_OCR_SELECTS_SECONDARY_RED_BORDER_COLOR = 0x20,
    BRA_OCR_SELECTS_INTENSIFIED_OR_SECONDARY_GREEN = 0x10,
    BRA_OCR_SELECTS_SECONDARY_BLUE_BORDER_COLOR = 0x08,
    BRA_OCR_SELECTS_RED_BORDER_COLOR = 0x04,
    BRA_OCR_SELECTS_GREEN_BORDER_COLOR = 0x02,
    BRA_OCR_SELECTS_BLUE_BORDER_COLOR = 0x01
};

/*
    Attribute Controller Registers

    Color Plane Enable Register
*/
enum bus_reg_attr_bit_cper {
    BRA_CPER_VIDEO_STATUS_MUX_RANGE = 0x30,
    BRA_CPER_ENABLE_COLOR_PLANE_RANGE = 0x0F
};

/*
    Attribute Controller Registers

    Horizontal Pel Panning Register
*/
enum bus_reg_attr_bit_hppr {
    BRA_HPPR_HORIZONTAL_PEL_PANNING_RANGE = 0x0F
};

uint_8 busReadExternal(const enum bus_reg_external reg, const enum modeOfOperation mode);
uint_8 busReadCRT(const enum bus_reg_crt_index index, const enum modeOfOperation mode);

void busWriteExternal(const enum bus_reg_external reg, const uint_8 data, const enum modeOfOperation mode);
void busWriteSequencer(const enum bus_reg_sequencer_index index, const uint_8 data, const enum modeOfOperation mode);
void busWriteCRT(const enum bus_reg_crt_index index, const uint_8 data, const enum modeOfOperation mode);
void busWriteGraphicsPosition(const struct bus_reg_graphics_position position);
void busWriteGraphics(const enum bus_reg_graphics_index index, const uint_8 data, const enum modeOfOperation mode);
void busWriteAttribute(const enum bus_reg_attr_index index, const uint_8 data, const enum modeOfOperation mode);

#endif // VGA_BUS_H
