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
//

#include "sequencer.h"
#include "../ega_bus.h"

struct registers {
    uint_8 reset;
    uint_8 clockingMode;
    uint_8 mapMask;
    uint_8 charMapSelect;
    uint_8 memoryMode;
};

static void push(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteSequencer(BRSI_RESET, reg->reset, mode);
    busWriteSequencer(BRSI_CLOCKING_MODE, reg->clockingMode, mode);
    busWriteSequencer(BRSI_MAP_MASK, reg->mapMask, mode);
    busWriteSequencer(BRSI_CHAR_MAP_SELECT, reg->charMapSelect, mode);
    busWriteSequencer(BRSI_MEMORY_MODE, reg->memoryMode, mode);
}

void sequencerSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;

    reg.reset = 0x03;
    reg.charMapSelect = 0x00;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1: {
        reg.clockingMode = 0x0B;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x03;
    } break;
    case CD_OPMODE_2:
    case CD_OPMODE_3: {
        reg.clockingMode = 0x01;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x03;
    } break; 
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        reg.clockingMode = 0x0B;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x02;
    } break;
    case CD_OPMODE_6: {
        reg.clockingMode = 0x01;
        reg.mapMask      = 0x01;
        reg.memoryMode   = 0x06;
    } break;
    case CD_OPMODE_D: {
        reg.clockingMode = 0x0B;
        reg.mapMask      = 0x0F;
        reg.memoryMode   = 0x06;
    } break;
    case CD_OPMODE_E: {
        reg.clockingMode = 0x01;
        reg.mapMask      = 0x0F;
        reg.memoryMode   = 0x06;
    } break;
    case MD_OPMODE_7: {
        reg.clockingMode = 0x00;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x03;
    } break;
    case MD_OPMODE_F: {
        reg.clockingMode = memOver64K ? 0x01 : 0x05;
        reg.mapMask      = 0x0F;
        reg.memoryMode   = memOver64K ? 0x06 : 0x00;
    } break;
    case ECD_OPMODE_0:
    case ECD_OPMODE_1: {
        reg.clockingMode = 0x0B;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x03;
    } break;
    case ECD_OPMODE_2:
    case ECD_OPMODE_3: {
        reg.clockingMode = 0x01;
        reg.mapMask      = 0x03;
        reg.memoryMode   = 0x03;
    } break;
    case ECD_OPMODE_10: {
        reg.clockingMode = memOver64K ? 0x01 : 0x05;
        reg.mapMask      = 0x0F;
        reg.memoryMode   = memOver64K ? 0x06 : 0x00;
    } break;
    }

    push(&reg, mode);
}