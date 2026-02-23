//
//  keyboard.c
//  GunwOS
//
//  Created by Artur Danielewski on 30.07.2025.  
//

#include "keyboard.h"
#include "data.h"
#include <gunwdrv.h>
#include <gunwoutput.h>
#include <gunwfug.h>
#include <gunwdevemitter.h>
#include <gunwinput.h>

extern const size_t KEYBOARD_DEVICE_ID;

void init_keyboard() {
    drvInitReport(KEYBOARD_DEVICE_ID, true);
}

void start_keyboard() {
    drvStartReport(KEYBOARD_DEVICE_ID, true);
}

void emitKeyboardEvent(const char key) {

    int_32 type;
    char data;

    if (key & 0b10000000) {
        type = GKEC_KEY_UP;
        data = key & 0b01111111;
    }
    else {
        type = GKEC_KEY_DOWN;
        data = key;
    }

    enum gnwDeviceError err;
    struct gnwDeviceEvent event;
    event.type = type;
    event.data = (ptr_t)&data;
    event.dataSizeBytes = sizeof(char);

    err = emit(KEYBOARD_DEVICE_ID, &event);
    if (err == GDE_NOT_FOUND) {
        logn("Keyboard event ignored - no listener");
    } else if (err != GDE_NONE) {
        logn("Error emitting keyboard event");
        fug(FUG_UNDEFINED);
    }
}
