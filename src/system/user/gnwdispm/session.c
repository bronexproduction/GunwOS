//
//  session.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.11.2023.
//

#include "session.h"

#include <defs.h>
#include <proc.h>
#include <gunwfug.h>

const sessionPtr_t displayStack[MAX_SESSION][MAX_DISPLAY];
struct session sessions[MAX_SESSION];

sessionPtr_t sessionForProc(const procId_t procId) {
    for (size_t index = 0; index < MAX_SESSION; ++index) {
        if (sessions[index].procId == procId) {
            return &sessions[index];
        }
    }
    
    return nullptr;
}

bool sessionIsOnTop(const sessionPtr_t sessionPtr) {
    return *displayStack[sessionPtr->displayDescriptor.identifier] == sessionPtr;
}

static enum gnwDeviceError setDisplayFormat(const size_t deviceId, const enum gnwDeviceUHA_display_format format) {
    struct gnwDeviceParamDescriptor paramDesc;

    paramDesc.param = GDU_DISPLAY_PARAM_FORMAT;

    return devSetParam(deviceId, &paramDesc, format);
}

enum gnwDeviceError sessionCreate(const procId_t procId, 
                                  const struct gnwDisplayDescriptor * const displayDescriptor, 
                                  const sessionPtr_t * sessionPtr) {
    
    CHECKPTR(displayDescriptor);
    CHECKPTR(sessionPtr);

    /*
        Find free session
    */
    sessionPtr_t freeSession = nullptr;
    for (size_t i = 0; i < MAX_SESSION; ++i) {
        if (sessions[i].procId == NONE_PROC_ID) {
            freeSession = &sessions[i];
            break;
        }
    }
    if (!freeSession) {
        return GDE_OPERATION_FAILED;
    }

    /*
        Configure session values
    */
    freeSession->procId = procId;
    freeSession->displayDescriptor = *displayDescriptor;

    /*
        Set session return value
    */
    *sessionPtr = freeSession;

    return GDE_NONE;
}

enum gnwDeviceError sessionEnable(const sessionPtr_t sessionPtr) {

    CHECKPTR(sessionPtr);

    sessionPtr_t * stack = displayStack[sessionPtr->displayDescriptor.identifier];

    /*
        Check if session already on display stack
    */
    size_t sessionIndex = 0;
    for (; sessionIndex < MAX_SESSION; ++sessionIndex) {
        if (stack[sessionIndex] == sessionPtr) {
            break;
        }
    }

    if (sessionIndex == MAX_SESSION && stack[MAX_SESSION - 1]) {
        /*
            Display stack full
        */
        return GDE_OPERATION_FAILED;
    }

    /*
        Shift stack - TBD - take sessionIndex into consideration
    */
    for (size_t i = MAX_SESSION - 1; i > 0; --i) {
        if (stack[i-1] != sessionPtr) {
            stack[i] = stack[i-1];
        }
    }

    /*
        Add session at the beginning
    */

    /*
        Set display adapter format
    */

    enum gnwDeviceError e = setDisplayFormat(sessionPtr->displayDescriptor.identifier, 
                                             sessionPtr->displayDescriptor.format);
    if (e) {
        return e;
    }

    return GDE_NONE;
}

void sessionDestroy(const sessionPtr_t sessionPtr) {
    /*
        TO BE DETERMINED   
    */
    /*
        SHIFT DISPLAY STACK
    */
}
