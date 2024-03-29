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

#define DISPMGR_LISTENER(NAME, PREPARE_RESULT, PREPARE_BINDING, PREPARE_PERMISSIONS) static void ipc ## NAME ## Listener(const struct gnwIpcEndpointQuery * const query) {              \
    if (!query) { fug(FUG_NULLPTR); return; }                                                                                                                                           \
    if (!query->data.ptr) { fug(FUG_INCONSISTENT); return; }                                                                                                                            \
    if (query->data.bytes != sizeof(struct gnwDisplayManager ## NAME ## Query)) { fug(FUG_INCONSISTENT); return; }                                                                      \
    if (query->replySizeBytes != sizeof(struct gnwDisplayManager ## NAME ## Result)) { fug(FUG_INCONSISTENT); return; }                                                                 \
    struct gnwDisplayManager ## NAME ## Query * const dispQueryPtr = (struct gnwDisplayManager ## NAME ## Query *)query->data.ptr;                                                      \
    struct gnwDisplayManager ## NAME ## Result result; { PREPARE_RESULT; }                                                                                                              \
    enum gnwIpcBindFlag bindFlag = GIBF_NONE; { PREPARE_BINDING; }                                                                                                                      \
    size_t permissions = 0; { PREPARE_PERMISSIONS; }                                                                                                                                    \
    enum gnwIpcError error = ipcReply((data_t){ (ptr_t)&result, sizeof(struct gnwDisplayManager ## NAME ## Result) }, query->token, (struct gnwIpcBindData){ bindFlag, permissions });  \
    if (error == GIPCE_NOT_FOUND) { return; }                                                                                                                                           \
    else if (error != GIPCE_NONE) { fug(FUG_UNDEFINED); return; }                                                                                                                       \
}

DISPMGR_LISTENER(GetDisplay, {
    result.error = display_getDisplay(dispQueryPtr->type, &result.displayDescriptor);
}, {}, {})

DISPMGR_LISTENER(AttachToDisplay, {
    result.error = display_attachToDisplay(query->sourceProcId, dispQueryPtr->type, &dispQueryPtr->displayDescriptor);
}, {
    if (result.error == GDE_NONE) {
        bindFlag = GIBF_BIND;
    }
}, {})

DISPMGR_LISTENER(PushFrame, {
    result.error = display_pushFrame(query->sourceProcId, dispQueryPtr->displayId, dispQueryPtr->frameBuffer, dispQueryPtr->inputRange);
}, {}, {})

static void onSessionDestroy(const struct gnwIpcEndpointQuery * const query) {
    if (!query) { fug(FUG_NULLPTR); return; }
    if (!query->data.ptr) { fug(FUG_INCONSISTENT); return; }
    if (query->data.bytes != sizeof(procId_t)) { fug(FUG_INCONSISTENT); return; }

    const procId_t * const procIdPtr = (procId_t *)query->data.ptr;
    const sessionPtr_t session = sessionForProc(*procIdPtr);
    
    if (session) {
        sessionDestroy(session);
    }
}

void dupa() {
    if (!display_init()) {
        fug(FUG_UNDEFINED);
    }
    
    enum gnwIpcError e;
    
    e = ipcRegister(DISPMGR_PATH_GET, ipcGetDisplayListener, false, 0);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }
    e = ipcRegister(DISPMGR_PATH_ATTACH, ipcAttachToDisplayListener, false, 0);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    ipcSessionDestroyListener = onSessionDestroy;
    e = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED, ipcSessionDestroyListener);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    e = ipcRegister(DISPMGR_PATH_PUSH, ipcPushFrameListener, true, 0);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    runLoopStart();
}
