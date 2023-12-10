//
//  manager.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.05.2023.
//

#include <gunwipc.h>
#include <gunwfug.h>
#include <gunwrlp.h>
#include <dispmgr.h>

#include "display.h"
#include "session.h"

#define DISPMGR_LISTENER(NAME, PREPARE_RESULT, PREPARE_BINDING, PREPARE_PERMISSIONS) static void ipc ## NAME ## Listener(const struct gnwIpcEndpointQuery * const query) {  \
    if (!query) { fug(FUG_NULLPTR); return; }                                                                                                                               \
    if (!query->dataPtr) { fug(FUG_INCONSISTENT); return; }                                                                                                                 \
    if (query->dataSizeBytes != sizeof(struct gnwDisplayManager ## NAME ## Query)) { fug(FUG_INCONSISTENT); return; }                                                       \
    if (query->replySizeBytes != sizeof(struct gnwDisplayManager ## NAME ## Result)) { fug(FUG_INCONSISTENT); return; }                                                     \
    struct gnwDisplayManager ## NAME ## Query * const dispQueryPtr = (struct gnwDisplayManager ## NAME ## Query *)query->dataPtr;                                           \
    struct gnwDisplayManager ## NAME ## Result result; { PREPARE_RESULT; }                                                                                                  \
    enum gnwIpcBindFlag bindFlag = GIBF_NONE; { PREPARE_BINDING; }                                                                                                          \
    size_t permissions = 0; { PREPARE_PERMISSIONS; }                                                                                                                        \
    enum gnwIpcError error = ipcReply((ptr_t)&result, sizeof(struct gnwDisplayManager ## NAME ## Result), query->token, bindFlag, permissions);                             \
    if (error == GIPCE_NOT_FOUND) { return; }                                                                                                                               \
    else if (error != GIPCE_NONE) { fug(FUG_UNDEFINED); return; }                                                                                                           \
}

DISPMGR_LISTENER(GetDisplay, {
    result.error = display_getDisplay(dispQueryPtr->type, &result.displayDescriptor);
}, {}, {})

DISPMGR_LISTENER(AttachToDisplay, {
    result.error = display_attachToDisplay(query->sourceProcId, dispQueryPtr->type, &dispQueryPtr->displayDescriptor);
}, {
    if (result.error != GDE_NONE) {
        bindFlag = GIBF_BIND;
    }
}, {})

DISPMGR_LISTENER(PushFrame, {
    result.error = display_pushFrame(query->sourceProcId, dispQueryPtr->displayId, dispQueryPtr->frameBuffer, dispQueryPtr->inputRange);
}, {}, {})

static void ipcProcessKilledListener(const struct gnwIpcEndpointQuery * const query) {
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
    if (!display_init()) {
        fug(FUG_UNDEFINED);
    }
    ipcRegister(DISPMGR_PATH_GET, ipcGetDisplayListener, false, 0);
    ipcRegister(DISPMGR_PATH_ATTACH, ipcAttachToDisplayListener, false, 0);

    ipcRegister(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED, ipcProcessKilledListener, false, 0);

    ipcRegister(DISPMGR_PATH_PUSH, ipcPushFrameListener, true, 0);

    runLoopStart();
}
