//
//  external.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "External Registers" - page 12
//  Section "Programming the Registers" - page 63
//

#include "external.h"
#include "../ega_bus.h"

struct registers {
    uint_8 miscOutput;
    uint_8 featureControl;
};

static void push(const struct registers * const reg, const enum modeOfOperation mode) {
    busWriteExternal(BRE_MISC_OUT, reg->miscOutput, mode);
    busWriteExternal(BRE_FEATURE_CTRL, reg->featureControl, mode);
}

void externalSetMode(const enum modeOfOperation mode) {

    struct registers reg;

    reg.featureControl = 0x00;

    switch (mode) {
    case CD_OPMODE_0:
    case CD_OPMODE_1:
    case CD_OPMODE_2:
    case CD_OPMODE_3:
    case CD_OPMODE_4:
    case CD_OPMODE_5:
    case CD_OPMODE_6:
    case CD_OPMODE_D:
    case CD_OPMODE_E: {
        reg.miscOutput = 0x23;
    } break;
    case MD_OPMODE_7: {
        reg.miscOutput = 0xA6;
    } break;
    case MD_OPMODE_F: {
        reg.miscOutput = 0xA2;
    } break;
    case ECD_OPMODE_0:
    case ECD_OPMODE_1:
    case ECD_OPMODE_2:
    case ECD_OPMODE_3:
    case ECD_OPMODE_10: {
        reg.miscOutput = 0xA7;
    } break;
    }

    push(&reg, mode);
}