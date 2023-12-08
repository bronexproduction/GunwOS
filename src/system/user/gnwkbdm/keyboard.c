//
//  keyboard.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#include "keyboard.h"
#include "session.h"

#include <mem.h>
#include <proc.h>
#include <gunwkeyboard.h>
#include <gunwfug.h>

extern sessionPtr_t displayStack[MAX_SESSION][MAX_DISPLAY];
extern struct session sessions[MAX_SESSION];
extern void sessionClear(const sessionPtr_t sessionPtr);

static GNW_KEYBOARD_EVENT_LISTENER(onKeyboardEvent) {
    if (event->code == GKEC_KEY_DOWN) {
        /*
            Send key down event
        */
    } else if (event->code == GKEC_KEY_UP) {
        /*
            Send key up event
        */
    } else {
        fug(FUG_INCONSISTENT);
    }
}

bool display_init() {
    memzero(displayStack, sizeof(sessionPtr_t) * MAX_SESSION * MAX_DISPLAY);
    for (size_t i = 0; i < MAX_SESSION; ++i) {
        sessionClear(&sessions[i]);
    }
    
    struct gnwDeviceUHADesc keyboardDesc;
    enum gnwDeviceError e = devGetByType(DEV_TYPE_KEYBOARD, &keyboardDesc);

    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    e = devAcquire(keyboardDesc.identifier);
    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    e = devListen(keyboardDesc.identifier, (gnwDeviceEventListener)onKeyboardEvent);
    if (e != GDE_NONE) {
        devRelease(keyboardDesc.identifier);
        fug(FUG_UNDEFINED);
        return false;
    }

    return true;
}
