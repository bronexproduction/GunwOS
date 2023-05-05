//
//  sequencer.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Sequencer Registers" - page 18
//  Section "Programming the Registers" - page 63
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "sequencer.h"
#include "../vga_bus.h"
#include <error/panic.h>

struct registers {
    uint_8 clockingMode;
    uint_8 mapMask;
    uint_8 charMapSelect;
    uint_8 memoryMode;
};

static void pushConfig(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteSequencer(BRSI_CLOCKING_MODE, reg->clockingMode, mode);
    busWriteSequencer(BRSI_MAP_MASK, reg->mapMask, mode);
    busWriteSequencer(BRSI_CHAR_MAP_SELECT, reg->charMapSelect, mode);
    busWriteSequencer(BRSI_MEMORY_MODE, reg->memoryMode, mode);
}

void sequencerDisable(const enum modeOfOperation mode) {
    busWriteSequencer(BRSI_RESET, 0x00, mode);
}

void sequencerSetMode(const enum modeOfOperation mode) {

    struct registers reg;

    reg.charMapSelect = 0x00;

    switch (mode) {
    case VGA_OPMODE_3: {
        reg.clockingMode = BRS_CMR_8_9_DOT_CLOCKS; /* 0x01 */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY | BRS_MEMMODEREG_ALPHA; /* 0x03 */
    } break;
    case VGA_OPMODE_4: {
        reg.clockingMode = BRS_CMR_DOT_CLOCK | BRS_CMR_BANDWIDTH | BRS_CMR_8_9_DOT_CLOCKS; /* 0x0B */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY; /* 0x02 */
    } break;
    default: {
        OOPS("Unsupported video mode");
    } break;
    }

    pushConfig(&reg, mode);
}

void sequencerEnable(const enum modeOfOperation mode) {
    busWriteSequencer(BRSI_RESET, BRS_RR_SYNCHRONOUS_RESET | BRS_RR_ASYNCHRONOUS_RESET /* 0x03 */, mode);
}
