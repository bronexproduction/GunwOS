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

void sequencerDisable(const enum modeOfOperation mode, const bool memOver64K) {
    #warning TO BE IMPLEMENTED
}

void sequencerSetMode(const enum modeOfOperation mode, const bool memOver64K) {

    struct registers reg;

    reg.reset = BRS_RR_SYNCHRONOUS_RESET | BRS_RR_ASYNCHRONOUS_RESET; /* 0x03 */;
    reg.charMapSelect = 0x00;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1:
    case ECD_OPMODE_0:
    case ECD_OPMODE_1: {
        reg.clockingMode = BRS_CMR_DOT_CLOCK | BRS_CMR_BANDWIDTH | BRS_CMR_8_9_DOT_CLOCKS; /* 0x0B */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY | BRS_MEMMODEREG_ALPHA /* 0x03 */;
    } break;
    case CD_OPMODE_2:
    case CD_OPMODE_3:
    case ECD_OPMODE_2:
    case ECD_OPMODE_3: {
        reg.clockingMode = BRS_CMR_8_9_DOT_CLOCKS; /* 0x01 */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY | BRS_MEMMODEREG_ALPHA; /* 0x03 */
    } break;
    case CD_OPMODE_4:
    case CD_OPMODE_5: {
        reg.clockingMode = BRS_CMR_DOT_CLOCK | BRS_CMR_BANDWIDTH | BRS_CMR_8_9_DOT_CLOCKS; /* 0x0B */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY; /* 0x02 */
    } break;
    case CD_OPMODE_6: {
        reg.clockingMode = BRS_CMR_8_9_DOT_CLOCKS; /* 0x01 */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x01 */
        reg.memoryMode   = BRS_MEMMODEREG_ODD_EVEN | BRS_MEMMODEREG_EXTENDED_MEMORY; /* 0x06 */
    } break;
    case CD_OPMODE_D: {
        reg.clockingMode = BRS_CMR_DOT_CLOCK | BRS_CMR_BANDWIDTH | BRS_CMR_8_9_DOT_CLOCKS; /* 0x0B */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_3 | BRS_MAPMASKREG_ENABLE_MAP_2 | BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x0F */
        reg.memoryMode   = BRS_MEMMODEREG_ODD_EVEN | BRS_MEMMODEREG_EXTENDED_MEMORY; /* 0x06 */
    } break;
    case CD_OPMODE_E: {
        reg.clockingMode = BRS_CMR_8_9_DOT_CLOCKS; /* 0x01 */
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_3 | BRS_MAPMASKREG_ENABLE_MAP_2 | BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x0F */
        reg.memoryMode   = BRS_MEMMODEREG_ODD_EVEN | BRS_MEMMODEREG_EXTENDED_MEMORY; /* 0x06 */
    } break;
    case MD_OPMODE_7: {
        reg.clockingMode = 0x00;
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x03 */
        reg.memoryMode   = BRS_MEMMODEREG_EXTENDED_MEMORY | BRS_MEMMODEREG_ALPHA; /* 0x03 */
    } break;
    case MD_OPMODE_F:
    case ECD_OPMODE_10: {
        reg.clockingMode = memOver64K ? 
                           BRS_CMR_8_9_DOT_CLOCKS /* 0x01 */ : 
                           BRS_CMR_SHIFT_LOAD | BRS_CMR_8_9_DOT_CLOCKS /* 0x05 */;
        reg.mapMask      = BRS_MAPMASKREG_ENABLE_MAP_3 | BRS_MAPMASKREG_ENABLE_MAP_2 | BRS_MAPMASKREG_ENABLE_MAP_1 | BRS_MAPMASKREG_ENABLE_MAP_0; /* 0x0F */
        reg.memoryMode   = memOver64K ? 
                           BRS_MEMMODEREG_ODD_EVEN | BRS_MEMMODEREG_EXTENDED_MEMORY /* 0x06 */ : 
                           0x00;
    } break;
    }

    push(&reg, mode);
}

void sequencerEnable(const enum modeOfOperation mode, const bool memOver64K) {
    #warning TO BE IMPLEMENTED
}
