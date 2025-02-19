//
//  mouse.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2025.  
//

#include "ops.h"
#include <gunwdrv.h>
#include <gunwbus.h>
#include <gunwlog.h>
#include <gunwfug.h>

#define TRY(OP, ON_FAIL) if (!OP()) { ON_FAIL; return; }
#define INIT_TRY(OP) TRY(OP, drvInitReport(false))
#define START_TRY(OP) TRY(OP, drvStartReport(false))

void init_mouse() {
    INIT_TRY(inputEnable);
    INIT_TRY(reset);
    INIT_TRY(setDefaults);

    drvInitReport(true);
}

void start_mouse() {
    START_TRY(enablePacketStreaming);
    START_TRY(enableInterrupt);

    drvStartReport(true);
}
