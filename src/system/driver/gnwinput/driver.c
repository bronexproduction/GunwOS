//
//  driver.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.02.2025.  
//

#include <gunwdrv.h>
#include <gunwfug.h>
#include <gunwlog.h>
#include <gunwbus.h>
#include "data.h"
#include "ops.h"
#include "keyboard.h"
#include "mouse.h"

#warning TODO https://wiki.osdev.org/%228042%22_PS/2_Controller

static void isr_keyboard() {
    /* Checking output buffer status */
    const uint_8 status = rdb(KEYBOARD_DEVICE_ID, BA_STATUS);
    if (!(status & CSR_OUTPUT_BUFFER_FULL)) {
        /*
            No data
        */
        log("Keyboard output buffer empty on keyboard interrupt");
        return;
    }
    if ((status & CSR_AUXILIARY_OUTPUT_BUFFER_FULL)) {
        /*
            Mouse input
        */
        log("Mouse data available on keyboard interrupt");
        return;
    }

    /* Reading keycode */
    uint_8 c = rdb(KEYBOARD_DEVICE_ID, BA_DATA);
    
    emitKeyboardEvent(c);
}

static void isr_mouse() {
    /* Checking output buffer status */
    const uint_8 status = rdb(MOUSE_DEVICE_ID, BA_STATUS);
    if (!(status & CSR_OUTPUT_BUFFER_FULL)) {
        /*
            No data
        */
        return;
    }
    if (!(status & CSR_AUXILIARY_OUTPUT_BUFFER_FULL)) {
        /*
            Keyboard input
        */
        log("Keyboard data available on mouse interrupt");
        return;
    }

    struct eventDataPacket data;
    if (!readEventData(&data)) {
        log("Unable to get event data");
        fug(FUG_INCONSISTENT);
        return;
    }

    emitMouseEvents(data);
}

#define DESCRIPTOR_COUNT 2

const size_t _gnw_device_descriptor_count = DESCRIPTOR_COUNT;
const struct gnwDeviceDescriptor _gnw_device_descriptor_list[DESCRIPTOR_COUNT] = {
    /* keyboard */ {
        /* type */ DEV_TYPE_KEYBOARD,
        /* api */ {
            GNW_UHA_NO_SYSTEM,
            GNW_UHA_NO_MEM,
            /* keyboard */ {
                /* desc */ {
                    /* _unused */ 0
                },
                /* routine */ {
                    /* _unused */ nullptr
                }
            },
            GNW_UHA_NO_MOUSE,
            GNW_UHA_NO_FDC,
            GNW_UHA_NO_STORCTRL,
            GNW_UHA_NO_CHAR_IN,
            GNW_UHA_NO_CHAR_OUT,
            GNW_UHA_NO_DISPLAY,
            GNW_UHA_NO_EVENT
        },
        /* driver */ {
            /* io */ {
                /* busBase */ BA_DATA,
            },
            /* descriptor */ {
                /* init */ init_keyboard,
                /* start */ start_keyboard,
                /* isr */ isr_keyboard,
                /* IRQ */ IRQ_KEYBOARD
            }
        },
        /* name */ "Keyboard driver for 8042 PS/2 controller"
    },
    /* mouse */ {
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
                /* busBase */ BA_DATA,
            },
            /* descriptor */ {
                /* init */ init_mouse,
                /* start */ start_mouse,
                /* isr */ isr_mouse,
                /* IRQ */ IRQ_MOUSE
            }
        },
        /* name */ "Mouse driver for 8042 PS/2 controller"
    }
};
const size_t _gnw_device_identifier_list[DESCRIPTOR_COUNT];
