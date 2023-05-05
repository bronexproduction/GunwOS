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
        uint_8 crtContextMCR;
        uint_8 crtContextVRE;

        /*
            #warning disable display
            #warning unlock registers (CRTC)
        */
        crtDisable(mode, &crtContextMCR, &crtContextVRE);
        sequencerDisable(mode);
        externalDisable(mode);

        /*
            Loading registers
        */
        externalSetMode(mode, &externalContext);
        sequencerSetMode(mode);
        crtSetMode(mode, &crtContextMCR, &crtContextVRE);
        graphicsSetMode(mode);
        attributeSetMode(mode);
        
        /*
            #warning lock CRTC registers
            #warning enable display
        */
        externalEnable(mode, externalContext);
        sequencerEnable(mode);
        attributeEnable(mode);
        crtEnable(mode, crtContextMCR, crtContextVRE);
    } break;
    default:
        OOPS("Unsupported display mode");
        return;
    }
}
