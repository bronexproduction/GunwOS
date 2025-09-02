//
//  gunwinput.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include <gunwinput.h>
#include <gunwfug.h>
#include "../include/gunwipc.h"
#include <inputmgr.h>

static gnwKeyboardEventListener keyboardEventListener;
static gnwMouseEventListener mouseEventListener;

static void ipcKeyboardEventListener(const struct gnwIpcEndpointQuery * const query) {
    if (!keyboardEventListener) {
        fug(FUG_NULLPTR);
        return;
    }
    if (!query) { 
        fug(FUG_NULLPTR); 
        return; 
    }
    if (!query->data.ptr || query->data.bytes != sizeof(struct gnwInputManagerKeyboardEventQuery)) {
        fug(FUG_INCONSISTENT);
        return;
    }

    struct gnwInputManagerKeyboardEventQuery * const eventQueryPtr = (struct gnwInputManagerKeyboardEventQuery *)query->data.ptr;
    
    keyboardEventListener(&eventQueryPtr->keyboardEvent);
}

static void ipcMouseEventListener(const struct gnwIpcEndpointQuery * const query) {
    if (!mouseEventListener) {
        fug(FUG_NULLPTR);
        return;
    }
    if (!query) { 
        fug(FUG_NULLPTR); 
        return; 
    }
    if (!query->data.ptr || query->data.bytes != sizeof(struct gnwInputManagerMouseEventQuery)) {
        fug(FUG_INCONSISTENT);
        return;
    }

    struct gnwInputManagerMouseEventQuery * const eventQueryPtr = (struct gnwInputManagerMouseEventQuery *)query->data.ptr;
    
    mouseEventListener(&eventQueryPtr->mouseEvent);
}

static enum gnwDeviceError registerKeyboardListener(const gnwKeyboardEventListener listener) {
    if (!listener) {
        return GDE_INVALID_PARAMETER;
    }

    enum gnwIpcError error = ipcRegister(INPUTMGR_PATH_KEYBOARD_EVENT, ipcKeyboardEventListener, true, 0);
    if (error != GIPCE_NONE) {
        return GDE_INVALID_OPERATION;
    }

    keyboardEventListener = listener;
    return GDE_NONE;
}

static enum gnwDeviceError registerMouseListener(const gnwMouseEventListener listener) {
    if (!listener) {
        return GDE_INVALID_PARAMETER;
    }

    enum gnwIpcError error = ipcRegister(INPUTMGR_PATH_MOUSE_EVENT, ipcMouseEventListener, true, 0);
    if (error != GIPCE_NONE) {
        return GDE_INVALID_OPERATION;
    }

    mouseEventListener = listener;
    return GDE_NONE;
}

enum gnwDeviceError attachToInput(const gnwKeyboardEventListener keyboardListener,
                                  const gnwMouseEventListener mouseListener) {
    if (!ipcSessionDestroyListener) {
        return GDE_PRECONDITION_NOT_SATISFIED;
    }

    enum gnwIpcError error = ipcRegisterNotification(GNW_PATH_IPC_BINDING_NOTIFICATION_SESSION_DESTROYED,
                                                     ipcSessionDestroyListener);
    if (error != GIPCE_NONE) {
        return GDE_INVALID_OPERATION;
    }

    struct gnwInputManagerAttachResult result;
    error = ipcSend(INPUTMGR_PATH_ATTACH,
                    (data_t){ nullptr, 0 },
                    (data_t){ (ptr_t)&result, sizeof(struct gnwInputManagerAttachResult) },
                    (struct gnwIpcBindData){ GIBF_BIND, 0 });
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }

    enum gnwDeviceError regError;
    regError = registerKeyboardListener(keyboardListener);
    if (regError != GDE_NONE) {
        return regError;
    }
    regError = registerMouseListener(mouseListener);
    if (regError != GDE_NONE) {
        return regError;
    }

    return GDE_NONE;
}

#endif // _GUNWAPI_KERNEL
