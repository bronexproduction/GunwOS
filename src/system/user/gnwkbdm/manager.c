//
//  manager.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#include <gunwipc.h>
#include <gunwfug.h>
#include <gunwrlp.h>
#include <kbdmgr.h>

#include "keyboard.h"
#include "session.h"

#define KBDMGR_LISTENER(NAME, PREPARE_RESULT, PREPARE_BINDING, PREPARE_PERMISSIONS) static void ipc ## NAME ## Listener(const struct gnwIpcEndpointQuery * const query) {   \
    if (!query) { fug(FUG_NULLPTR); return; }                                                                                                                               \
    if (!query->dataPtr) { fug(FUG_INCONSISTENT); return; }                                                                                                                 \
    if (query->dataSizeBytes != sizeof(struct gnwKeyboardManager ## NAME ## Query)) { fug(FUG_INCONSISTENT); return; }                                                      \
    if (query->replySizeBytes != sizeof(struct gnwKeyboardManager ## NAME ## Result)) { fug(FUG_INCONSISTENT); return; }                                                    \
    struct gnwKeyboardManager ## NAME ## Query * const kbdQueryPtr = (struct gnwKeyboardManager ## NAME ## Query *)query->dataPtr;                                         \
    struct gnwKeyboardManager ## NAME ## Result result; { PREPARE_RESULT; }                                                                                                 \
    enum gnwIpcBindFlag bindFlag = GIBF_NONE; { PREPARE_BINDING; }                                                                                                          \
    size_t permissions = 0; { PREPARE_PERMISSIONS; }                                                                                                                        \
    enum gnwIpcError error = ipcReply((ptr_t)&result, sizeof(struct gnwKeyboardManager ## NAME ## Result), query->token, bindFlag, permissions);                            \
    if (error == GIPCE_NOT_FOUND) { return; }                                                                                                                               \
    else if (error != GIPCE_NONE) { fug(FUG_UNDEFINED); return; }                                                                                                           \
}

KBDMGR_LISTENER(AttachToKeyboard, {
    (void)kbdQueryPtr;
    result.error = keyboard_attach(query->sourceProcId);
}, {
    if (result.error == GDE_NONE) {
        bindFlag = GIBF_BIND;
    }
}, {})

static void ipcSessionDestroyListener(const struct gnwIpcEndpointQuery * const query) {
    if (!query) { fug(FUG_NULLPTR); return; }
    if (!query->dataPtr) { fug(FUG_INCONSISTENT); return; }
    if (query->dataSizeBytes != sizeof(procId_t)) { fug(FUG_INCONSISTENT); return; }

    const procId_t * const procIdPtr = (procId_t *)query->dataPtr;
    const sessionPtr_t session = sessionForProc(*procIdPtr);
    
    if (session) {
        sessionDestroy(session);
    }
}

void dupa() {
    if (!keyboard_init()) {
        fug(FUG_UNDEFINED);
    }

    enum gnwIpcError e;

    e = ipcRegister(KBDMGR_PATH_ATTACH, ipcAttachToKeyboardListener, false, 0);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    e = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED, ipcSessionDestroyListener);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    runLoopStart();
}
