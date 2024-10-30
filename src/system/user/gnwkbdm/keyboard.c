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
#include <string.h>
#include <gunwdev.h>
#include <gunwipc.h>
#include <gunwkeyboard.h>
#include <gunwfug.h>
#include <gunwlog.h>
#include <kbdmgr.h>

extern sessionPtr_t keyboardStack[MAX_SESSION];
extern struct session sessions[MAX_SESSION];
extern void sessionClear(const sessionPtr_t sessionPtr);

static GNW_KEYBOARD_EVENT_LISTENER(onKeyboardEvent) {
    CHECKPTR(event);

    if (event->code != GKEC_KEY_DOWN &&
        event->code != GKEC_KEY_UP) {
        fug(FUG_INCONSISTENT);
        return;
    }

    struct session * const session = sessionOnTop();
    if (!session) {
        return;
    }

    {
        char msg[29] = "onKeyboardEvent - new event ";
        log(msg);
    }
    {
        char msg[24] = "  event code -         ";
        int2str((addr_t)event->code, msg + 13);
        log(msg);
    }
    {
        char msg[128] = "  event key -         ";
        uint2dec((addr_t)event->key, msg + 14);
        log(msg);
    }

    struct gnwKeyboardManagerEventQuery query;
    query.keyboardEvent = *event;

    const enum gnwIpcError e = ipcSendDirect(session->procId, KBDMGR_PATH_EVENT,
                                             (data_t){ (ptr_t)&query, sizeof(struct gnwKeyboardManagerEventQuery) },
                                             (data_t){ nullptr, 0 },
                                             (struct gnwIpcBindData){ GIBF_NONE, 0 });
    if (e == GIPCE_NONE) {
        return;
    }

    log("gnwkbdm - onKeyboardEvent - ipcSendDirect failed - destroying session");
    sessionDestroy(session);
    
    if (e == GIPCE_FORBIDDEN || e == GIPCE_NOT_FOUND) {
        return;
    } else {
        char logMsg[75] = "gnwkbdm - onKeyboardEvent - ipcSendDirect failure unexpected error        ";
        int2str(e, logMsg + 67);
        log(logMsg);
        fug(FUG_UNDEFINED);
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
