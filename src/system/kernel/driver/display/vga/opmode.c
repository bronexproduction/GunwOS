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
    case CD_OPMODE_2: 
    case CD_OPMODE_3: 
    case CD_OPMODE_4: 
    case CD_OPMODE_5: {
        #warning how to detect memory extensions?
        const bool memOver64K = false;

        uint_8 externalContext;
        uint_8 crtContextMCR;
        uint_8 crtContextVRE;

        /*
            #warning disable display
            #warning unlock registers (CRTC)
        */
        crtDisable(mode, memOver64K, &crtContextMCR, &crtContextVRE);
        sequencerDisable(mode, memOver64K);
        externalDisable(mode);

        /*
            Loading registers
        */
        externalSetMode(mode, &externalContext);
        sequencerSetMode(mode, memOver64K);
        crtSetMode(mode, memOver64K, &crtContextMCR, &crtContextVRE);
        graphicsSetMode(mode, memOver64K);
        attributeSetMode(mode, memOver64K);
        
        /*
            #warning lock CRTC registers
            #warning enable display
        */
        externalEnable(mode, externalContext);
        sequencerEnable(mode, memOver64K);
        crtEnable(mode, memOver64K, crtContextMCR, crtContextVRE);
    } break;
    default:
        OOPS("Unsupported display mode");
        return;
    }
}
