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
#include <gunwdev.h>
#include <gunwkeyboard.h>
#include <gunwfug.h>

extern sessionPtr_t keyboardStack[MAX_SESSION];
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

bool keyboard_init() {
    memzero(keyboardStack, sizeof(sessionPtr_t) * MAX_SESSION);
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

enum gnwDeviceError keyboard_attach(const procId_t procId) {
    struct session * keyboardSession = sessionForProc(procId);
    if (keyboardSession) {
        return GDE_ALREADY_HELD;
    }

    enum gnwDeviceError e = sessionCreate(procId, &keyboardSession);
    if (e != GDE_NONE) {
        return e;
    }
    if (!keyboardSession) {
        fug(FUG_NULLPTR);
        return GDE_UNKNOWN;
    }

    e = sessionEnable(keyboardSession);
    if (e != GDE_NONE) {
        sessionDestroy(keyboardSession);
        return e;
    }

    return GDE_NONE;
}
