//
//  privileged.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.11.2023.
//

#include "display.h"
#include "session.h"

#include <gunwfug.h>

enum gnwDeviceError display_pushFrame(const procId_t procId,
                                      const size_t displayId,
                                      const ptr_t buffer,
                                      const range_addr_t inputRange) {
    CHECKPTR(buffer)

    struct session * const displaySession = sessionForProc(procId);
    if (!displaySession) {
        return GDE_HANDLE_INVALID;
    }

    if (displaySession->displayDescriptor.identifier != displayId) {
        sessionDestroy(displaySession);
        return GDE_HANDLE_INVALID;
    }

    if (!sessionIsOnTop(displaySession)) {
        return GDE_BUSY;
    }

    /*
        After the top process switch
        its frame should get refreshed
        or the content from the last process may stay presented
    */

    const enum gnwDeviceError e = devMemWrite(displayId, buffer, inputRange);
    if (e != GDE_NONE) {
        sessionDestroy(displaySession);
    }

    return e;
}
