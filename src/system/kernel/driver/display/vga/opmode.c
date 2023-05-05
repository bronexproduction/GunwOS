//
//  opmode.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  For more information see:
//  IBM Enhanced Graphics Adapter
//  Section "Modes of operation"
//  page 5+
//  and
//  Intel® OpenSource HD Graphics Programmer’s Reference Manual (PRM)
//

#include "opmode.h"
#include "regmodules/external.h"
#include "regmodules/sequencer.h"
#include "regmodules/crt.h"
#include "regmodules/graphics.h"
#include "regmodules/attribute.h"
#include <error/panic.h>

void setMode(const enum modeOfOperation mode) {
    switch(mode) {
    case VGA_OPMODE_3: 
    case VGA_OPMODE_4: {
        uint_8 externalContext;
        uint_8 sequencerContextCMR;
        uint_8 crtContextMCR;
        uint_8 crtContextVSE;

        /*
            Disable display and unlock CRT registers
        */
        crtDisable(mode, &crtContextMCR, &crtContextVSE);
        sequencerDisable(&sequencerContextCMR);
        externalDisable(mode);

        /*
            Load registers
        */
        externalSetMode(mode, &externalContext);
        sequencerSetMode(mode, &sequencerContextCMR);
        crtSetMode(mode, &crtContextMCR, &crtContextVSE);
        graphicsSetMode(mode);
        attributeSetMode(mode);

        /*
            Enable display and lock CRT registers
        */
        externalEnable(mode, externalContext);
        sequencerEnable(sequencerContextCMR);
        crtEnable(mode, crtContextMCR, crtContextVSE);
        attributeEnable();
    } break;
    default:
        OOPS("Unsupported display mode");
        return;
    }
}
