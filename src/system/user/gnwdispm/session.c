//
//  session.c
//  GunwOS
//
//  Created by Artur Danielewski on 28.11.2023.
//

#include "session.h"

#include <defs.h>
#include <mem.h>
#include <proc.h>
#include <utils.h>
#include <gunwfug.h>

sessionPtr_t displayStack[MAX_SESSION][MAX_DISPLAY];
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
                                  sessionPtr_t * const sessionPtr) {
    
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
        Shift stack
    */
    for (sessionIndex = MIN(sessionIndex, MAX_SESSION - 1); sessionIndex > 0; --sessionIndex) {
        if (stack[sessionIndex-1] != sessionPtr) {
            stack[sessionIndex] = stack[sessionIndex-1];
        }
    }

    /*
        Add session at the beginning
    */
    stack[0] = sessionPtr;

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

void sessionClear(const sessionPtr_t sessionPtr) {
    memzero(sessionPtr, sizeof(struct session));
    sessionPtr->procId = NONE_PROC_ID;
}

void sessionDestroy(const sessionPtr_t sessionPtr) {

    CHECKPTR(sessionPtr);

    sessionPtr_t * stack = displayStack[sessionPtr->displayDescriptor.identifier];
    
    /*
        Look for the session on the stack
    */
    int sessionIndex = MAX_SESSION - 1;
    for (; sessionIndex >= 0; --sessionIndex) {
        if (stack[sessionIndex] == sessionPtr) {
            break;
        }
    }

    /*
        Shift stack
    */
    for (; IN_RANGE(0, sessionIndex, MAX_SESSION - 1); --sessionIndex) {
        stack[sessionIndex] = (sessionIndex == (MAX_SESSION - 1)) ? nullptr : stack[sessionIndex + 1];
    }
    
    sessionClear(sessionPtr);
}
