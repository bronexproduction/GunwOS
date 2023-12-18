//
//  gunwkeyboard.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include <gunwkeyboard.h>
#include <gunwfug.h>
#include "../include/gunwipc.h"
#include <kbdmgr.h>

static gnwKeyboardEventListener eventListener;

static void ipcKeyboardEventListener(const struct gnwIpcEndpointQuery * const query) {
    if (!eventListener) {
        fug(FUG_NULLPTR);
        return;
    }
    if (!query) { 
        fug(FUG_NULLPTR); 
        return; 
    }
    if (!query->dataPtr || query->dataSizeBytes != sizeof(struct gnwKeyboardManagerEventQuery)) {
        fug(FUG_INCONSISTENT);
        return;
    }

    struct gnwKeyboardManagerEventQuery * const eventQueryPtr = (struct gnwKeyboardManagerEventQuery *)query->dataPtr;
    
    eventListener(&eventQueryPtr->keyboardEvent);
}

enum gnwDeviceError attachToKeyboard(const gnwKeyboardEventListener listener) {
    struct gnwKeyboardManagerAttachToKeyboardQuery query;
    query._unused = 0;
    struct gnwKeyboardManagerAttachToKeyboardResult result;

    if (!ipcSessionDestroyListener) {
        return GDE_INVALID_PARAMETER;
    }

    enum gnwIpcError error = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED,
                                                     ipcSessionDestroyListener);
    if (error != GIPCE_NONE) {
        return GDE_INVALID_OPERATION;
    }

    error = ipcSend(KBDMGR_PATH_ATTACH,
                    (ptr_t)&query, sizeof(struct gnwKeyboardManagerAttachToKeyboardQuery),
                    (ptr_t)&result, sizeof(struct gnwKeyboardManagerAttachToKeyboardResult),
                    GIBF_BIND, 0);
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }

    error = ipcRegister(KBDMGR_PATH_EVENT, ipcKeyboardEventListener, true, 0);
    if (error != GIPCE_NONE) {
        return GDE_INVALID_OPERATION;
    }

    eventListener = listener;

    return GDE_NONE;
}

#endif // _GUNWAPI_KERNEL