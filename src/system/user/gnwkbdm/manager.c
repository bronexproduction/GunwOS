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

static void ipcAttachToKeyboardListener(const struct gnwIpcEndpointQuery * const query) {
    if (!query) { 
        fug(FUG_NULLPTR); 
        return; 
    }
    if (!query->dataPtr) {
        fug(FUG_INCONSISTENT);
        return;
    }
    if (query->dataSizeBytes != sizeof(struct gnwKeyboardManagerAttachToKeyboardQuery)) {
        fug(FUG_INCONSISTENT);
        return;
    }
    if (query->replySizeBytes != sizeof(struct gnwKeyboardManagerAttachToKeyboardResult)) {
        fug(FUG_INCONSISTENT);
        return;
    }

    struct gnwKeyboardManagerAttachToKeyboardQuery * const kbdQueryPtr = (struct gnwKeyboardManagerAttachToKeyboardQuery *)query->dataPtr;
    (void)kbdQueryPtr;
    struct gnwKeyboardManagerAttachToKeyboardResult result;
    
    result.error = keyboard_attach(query->sourceProcId);
    
    enum gnwIpcError error = ipcReply((ptr_t)&result,
                                      sizeof(struct gnwKeyboardManagerAttachToKeyboardResult),
                                      query->token,
                                      (result.error == GDE_NONE) ? GIBF_BIND : GIBF_NONE,
                                      0);
    if (error == GIPCE_NOT_FOUND) {
        return;
    }
    else if (error != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
        return;
    }

    /*
        Call IPC path for called procId in order to bind reverse endpoint
    */
}

static void onSessionDestroy(const struct gnwIpcEndpointQuery * const query) {
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

    ipcSessionDestroyListener = onSessionDestroy;
    e = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED, ipcSessionDestroyListener);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    runLoopStart();
}
