//
//  session.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#include "session.h"

#include <defs.h>
#include <mem.h>
#include <proc.h>
#include <utils.h>
#include <gunwdevtypes.h>
#include <gunwfug.h>

sessionPtr_t keyboardStack[MAX_SESSION];
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
    return *keyboardStack == sessionPtr;
}

enum gnwDeviceError sessionCreate(const procId_t procId,
                                  sessionPtr_t * const sessionPtr) {
    
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

    /*
        Set session return value
    */
    *sessionPtr = freeSession;

    return GDE_NONE;
}

enum gnwDeviceError sessionEnable(const sessionPtr_t sessionPtr) {

    CHECKPTR(sessionPtr);

    /*
        Check if session already on display stack
    */
    size_t sessionIndex = 0;
    for (; sessionIndex < MAX_SESSION; ++sessionIndex) {
        if (keyboardStack[sessionIndex] == sessionPtr) {
            break;
        }
    }

    if (sessionIndex == MAX_SESSION && keyboardStack[MAX_SESSION - 1]) {
        /*
            Display stack full
        */
        return GDE_OPERATION_FAILED;
    }

    /*
        Shift stack
    */
    for (sessionIndex = MIN(sessionIndex, MAX_SESSION - 1); sessionIndex > 0; --sessionIndex) {
        if (keyboardStack[sessionIndex-1] != sessionPtr) {
            keyboardStack[sessionIndex] = keyboardStack[sessionIndex-1];
        }
    }

    /*
        Add session at the beginning
    */
    keyboardStack[0] = sessionPtr;

    return GDE_NONE;
}

void sessionClear(const sessionPtr_t sessionPtr) {
    memzero(sessionPtr, sizeof(struct session));
    sessionPtr->procId = NONE_PROC_ID;
}

void sessionDestroy(const sessionPtr_t sessionPtr) {

    CHECKPTR(sessionPtr);
    
    /*
        Look for the session on the stack
    */
    int sessionIndex = MAX_SESSION - 1;
    for (; sessionIndex >= 0; --sessionIndex) {
        if (keyboardStack[sessionIndex] == sessionPtr) {
            break;
        }
    }

    /*
        Shift stack
    */
    for (; IN_RANGE(0, sessionIndex, MAX_SESSION - 1); --sessionIndex) {
        keyboardStack[sessionIndex] = (sessionIndex == (MAX_SESSION - 1)) ? nullptr : keyboardStack[sessionIndex + 1];
    }
    
    sessionClear(sessionPtr);
}
