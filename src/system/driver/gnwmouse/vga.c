//
//  mouse.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2025.  
//

#include <gunwdrv.h>

static void init() {
    drvInitReport(true);
}

static void start() {
    drvStartReport(true);
}

static void isr() {
    drvInterruptReturn();
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
            /* busBase */ 0x6969,
        },
        /* descriptor */ {
            /* init */ init,
            /* start */ start,
            /* isr */ isr,
            /* IRQ */ 12
        }
    },
    /* name */ "Default mouse"
};
