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

        /*
            #warning disable display
            #warning unlock registers (CRTC)
        */
        externalDisable(mode);
        sequencerDisable(mode, memOver64K);
        crtDisable(mode, memOver64K);
        graphicsDisable(mode, memOver64K);

        /*
            Loading registers
        */
        externalSetMode(mode);
        sequencerSetMode(mode, memOver64K);
        crtSetMode(mode, memOver64K);
        graphicsSetMode(mode, memOver64K);
        attributeSetMode(mode, memOver64K);
        
        /*
            #warning lock CRTC registers
            #warning enable display
        */
        externalEnable(mode);
        sequencerEnable(mode, memOver64K);
        crtEnable(mode, memOver64K);
        graphicsEnable(mode, memOver64K);
    } break;
    default:
        OOPS("Unsupported display mode");
        return;
    }
}
