//
//  gunwkeyboard.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.12.2023.
//

#ifndef _GUNWAPI_KERNEL

#include <gunwkeyboard.h>
#include "../include/gunwipc.h"
#include <kbdmgr.h>

enum gnwDeviceError attachToKeyboard(const gnwKeyboardEventListener listener) {
    struct gnwKeyboardManagerAttachToKeyboardQuery query;
    query._unused = 0;
    struct gnwKeyboardManagerAttachToKeyboardResult result;

    enum gnwIpcError error = ipcSend(KBDMGR_PATH_ATTACH,
                                     (ptr_t)&query, sizeof(struct gnwKeyboardManagerAttachToKeyboardQuery),
                                     (ptr_t)&result, sizeof(struct gnwKeyboardManagerAttachToKeyboardResult));
    if (error != GIPCE_NONE) {
        return GDE_OPERATION_FAILED;
    } else if (result.error != GDE_NONE) {
        return result.error;
    }

    return GDE_NONE;
}

#endif // _GUNWAPI_KERNEL
