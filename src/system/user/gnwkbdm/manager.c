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

    e = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED, ipcSessionDestroyListener);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    runLoopStart();
}
