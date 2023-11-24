//
//  privileged.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.11.2023.
//

#include "display.h"

#include <gunwfug.h>

enum gnwDeviceError display_pushFrame(const procId_t procId,
                                      const size_t displayId,
                                      const ptr_t buffer,
                                      const range_addr_t inputRange) {
    CHECKPTR(buffer)

    if (!processPermitted) {
        return GDE_HANDLE_INVALID;
    }
    if (!processOnTop) {
        return GDE_BUSY;
    }

    /*
        After the top process switch
        its frame should get refreshed
        or the content from the last process may stay presented
    */

    return devMemWrite(displayId, buffer, &inputRange);
}
