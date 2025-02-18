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

static void init() {
    INIT_TRY(inputEnable);
    INIT_TRY(reset);
    INIT_TRY(setDefaults);

    drvInitReport(true);
}

static void start() {
    START_TRY(enablePacketStreaming);
    START_TRY(enableInterrupt);

    drvStartReport(true);
}

static void isr() {
    /* Checking output buffer status */
    const uint_8 status = rdb(MOUSE_BUS_STATUS);
    if (!(status & MOUSE_STAT_OUTB)) {
        /*
            No data
        */
        log("Mouse output buffer empty on mouse interrupt");
        return;
    }
    if (!(status & MOUSE_STAT_AUXBF)) {
        /*
            Mouse input
        */
        log("Keyboard data available on keyboard interrupt");
        return;
    }

    struct movementDataPacket data;
    if (!readMovementData(&data)) {
        log("Unable to get movement data");
        fug(FUG_INCONSISTENT);
        return;
    }

    // /*
    //     Extracting exact keycode

    //     MSB contains information whether key was pressed or released
    // */
    // if (c & 0b10000000) {
    //     emitEvent(GKEC_KEY_UP, c & 0b01111111);
    // }
    // else {
    //     emitEvent(GKEC_KEY_DOWN, c);
    // }
}

const struct gnwDeviceDescriptor _gnw_device_descriptor = {
    /* type */ DEV_TYPE_MOUSE,
    /* api */ {
        GNW_UHA_NO_SYSTEM,
        GNW_UHA_NO_MEM,
        GNW_UHA_NO_KEYBOARD,
        /* mouse */ {
            /* desc */ {
                /* _unused */ 0
            },
            /* routine */ {
                /* _unused */ nullptr
            }
        },
        GNW_UHA_NO_FDC,
        GNW_UHA_NO_STORCTRL,
        GNW_UHA_NO_CHAR_IN,
        GNW_UHA_NO_CHAR_OUT,
        GNW_UHA_NO_DISPLAY,
        GNW_UHA_NO_EVENT
    },
    /* driver */ {
        /* io */ {
            /* busBase */ MOUSE_BUS_DATA,
        },
        /* descriptor */ {
            /* init */ init,
            /* start */ start,
            /* isr */ isr,
            /* IRQ */ 12
        }
    },
    /* name */ "Default PS/2 mouse"
};
