//
//  ega_bus.h
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Registers"
//  page 12+
//

#ifndef EGA_BUS_H
#define EGA_BUS_H

#include <uha/gunwuha_display_desc.h>

#define BUS_ADDR_MONOCHROME_MARK 0xB0
#define BUS_ADDR_MONOCHROME_MARK_MASK 0xF0
#define BUS_ADDR_COLOR_OFFSET 0x20

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

enum bus_reg_attr {
    BRA_ADDRESS = 0x3C0,
    BRA_DATA = 0x3C0
};

enum bus_reg_attr_index {
    BRAI_PALETTE_START = 0x00, /* 0x00 - 0x0F range for 16 palettes */
    BRAI_MODE_CONTROL = 0x10,
    BRAI_OVERSCAN_COLOR = 0x11,
    BRAI_COLOR_PLANE_ENABLE = 0x12,
    BRAI_HORIZONTAL_PEL_PANNING = 0x13
};

/*
    Bus register addresses that matches 0x*B* refer to monochrome display address range.
    Corresponding bytes for color display are offset by 0x020 (addresses 0x*D*)

    For more information see:
    IBM Enhanced Graphics Adapter
    page 12+
*/
uint_16 bus_addr(uint_16 baseAddr, enum gnwDeviceUHA_display_format format);

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
    BRE_MOR_IO_ADRESS_SELECT = 0x01
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

#warning CRT REGISTERS AND MORE

#endif // EGA_BUS_H
