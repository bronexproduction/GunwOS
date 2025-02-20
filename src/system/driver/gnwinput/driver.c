//
//  driver.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.02.2025.  
//

#include <gunwdrv.h>
#include <gunwkeyboard.h>
#include <gunwfug.h>
#include <gunwdevemitter.h>
#include <gunwlog.h>
#include <gunwbus.h>
#include "data.h"
#include "ops.h"

#warning TODO https://wiki.osdev.org/%228042%22_PS/2_Controller

static void emitKeyboardEvent(const int_32 type, const char data) {
    enum gnwDeviceError err;
    struct gnwDeviceEvent event;
    event.type = type;
    event.data = (ptr_t)&data;
    event.dataSizeBytes = sizeof(char);

    err = emit(KEYBOARD_DEVICE_ID, &event);
    if (err == GDE_NOT_FOUND) {
        log("Keyboard event ignored - no listener");
    } else if (err != GDE_NONE) {
        log("Error emitting keyboard event");
        fug(FUG_UNDEFINED);
    }
}

static void init_keyboard() {
    drvInitReport(KEYBOARD_DEVICE_ID, true);
}

static void start_keyboard() {
    drvStartReport(KEYBOARD_DEVICE_ID, true);
}

static void isr_keyboard () {
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
    
    /*
        Extracting exact keycode

        MSB contains information whether key was pressed or released
    */
    if (c & 0b10000000) {
        emitKeyboardEvent(GKEC_KEY_UP, c & 0b01111111);
    }
    else {
        emitKeyboardEvent(GKEC_KEY_DOWN, c);
    }
}

static void isr_mouse() {
    /* Checking output buffer status */
    const uint_8 status = rdb(MOUSE_DEVICE_ID, BA_STATUS);
    if (!(status & CSR_OUTPUT_BUFFER_FULL)) {
        /*
            No data
        */
        log("Mouse output buffer empty on mouse interrupt");
        return;
    }
    if (!(status & CSR_AUXILIARY_OUTPUT_BUFFER_FULL)) {
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

extern void init_mouse();
extern void start_mouse();

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
