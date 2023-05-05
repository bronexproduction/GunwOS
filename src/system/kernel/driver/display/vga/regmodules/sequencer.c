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
    uint_8 mapMask;
    uint_8 charMapSelect;
    uint_8 memoryMode;
};

static void pushConfig(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteSequencer(BRSI_MAP_MASK, reg->mapMask, mode);
    busWriteSequencer(BRSI_CHAR_MAP_SELECT, reg->charMapSelect, mode);
    busWriteSequencer(BRSI_MEMORY_MODE, reg->memoryMode, mode);
}

void sequencerDisable(const enum modeOfOperation mode, uint_8 * const regContextCMR) {
    *regContextCMR = BRS_CMR_SCREEN_OFF;

    busWriteSequencer(BRSI_RESET, 0x00, mode);
    busWriteSequencer(BRSI_CLOCKING_MODE, *regContextCMR, mode);
}

void sequencerSetMode(const enum modeOfOperation mode, uint_8 * const regContextCMR) {

    struct registers reg;

    reg.charMapSelect = 0x00;

    switch (mode) {
    case VGA_OPMODE_3: {
        *regContextCMR = CLEAR(BRS_CMR_8_9_DOT_CLOCKS);
        reg.mapMask    = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0;
        reg.memoryMode = BRS_MEMMODEREG_EXTENDED_MEMORY;
    } break;
    case VGA_OPMODE_4: {
        // *regContextCMR = SET(BRS_CMR_8_9_DOT_CLOCKS);
        // reg.mapMask    = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0;
        // reg.memoryMode = BRS_MEMMODEREG_EXTENDED_MEMORY;
    } break;
    default: {
        OOPS("Unsupported video mode");
    } break;
    }

    pushConfig(&reg, mode);
}

void sequencerEnable(const enum modeOfOperation mode, uint_8 regContextCMR) {
    regContextCMR &= ~BRS_CMR_SCREEN_OFF;

    busWriteSequencer(BRSI_CLOCKING_MODE, regContextCMR, mode);
    busWriteSequencer(BRSI_RESET, BRS_RR_SYNCHRONOUS_RESET | BRS_RR_ASYNCHRONOUS_RESET /* 0x03 */, mode);
}
