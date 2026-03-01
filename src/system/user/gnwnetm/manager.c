//
//  manager.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#include <gunwipc.h>
#include <gunwfug.h>
#include <gunwrlp.h>
#include <netmgr.h>

#include "network.h"

#define NETMGR_LISTENER(NAME, PREPARE_RESULT) static void ipc ## NAME ## Listener(const struct gnwIpcEndpointQuery * const query) {                                             \
    if (!query) { fug(FUG_NULLPTR); return; }                                                                                                                                   \
    if (!query->data.ptr) { fug(FUG_INCONSISTENT); return; }                                                                                                                    \
    if (query->data.bytes != sizeof(struct gnwNetworkManager ## NAME ## Query)) { fug(FUG_INCONSISTENT); return; }                                                              \
    if (query->replySizeBytes != sizeof(struct gnwNetworkManager ## NAME ## Result)) { fug(FUG_INCONSISTENT); return; }                                                         \
    struct gnwNetworkManager ## NAME ## Query * const netQueryPtr = (struct gnwNetworkManager ## NAME ## Query *)query->data.ptr;                                              \
    struct gnwNetworkManager ## NAME ## Result result; { PREPARE_RESULT; }                                                                                                      \
    enum gnwIpcError error = ipcReply((data_t){ (ptr_t)&result, sizeof(struct gnwNetworkManager ## NAME ## Result) }, query->token, (struct gnwIpcBindData){ GIBF_NONE, 0 });   \
    if (error == GIPCE_NOT_FOUND) { return; }                                                                                                                                   \
    else if (error != GIPCE_NONE) { fug(FUG_UNDEFINED); return; }                                                                                                               \
}

NETMGR_LISTENER(GetSomething, {
    (void)netQueryPtr;
    // result.error = display_getNetwork(netQueryPtr->type, &result.displayDescriptor);
})

void dupa() {
    if (!network_init()) {
        fug(FUG_UNDEFINED);
    }
    
    enum gnwIpcError e;
    
    e = ipcRegister(NETMGR_PATH_GET, ipcGetSomethingListener, false, 0);
    if (e != GIPCE_NONE) {
        fug(FUG_UNDEFINED);
    }

    runLoopStart();
}
