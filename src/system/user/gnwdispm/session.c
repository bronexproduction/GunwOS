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

const struct session * displayStack[MAX_SESSION][MAX_DISPLAY];
struct session sessions[MAX_SESSION];

struct session * sessionForProc(const procId_t procId) {
    for (size_t index = 0; index < MAX_SESSION; ++index) {
        if (sessions[index].procId == procId) {
            return &sessions[index];
        }
    }
    
    return nullptr;
}

bool sessionIsOnTop(const struct session * const sessionPtr) {
    return *displayStack[sessionPtr->displayDescriptor.identifier] == sessionPtr;
}

static enum gnwDeviceError setDisplayFormat(const size_t deviceId, const enum gnwDeviceUHA_display_format format) {
    struct gnwDeviceParamDescriptor paramDesc;

    paramDesc.param = GDU_DISPLAY_PARAM_FORMAT;

    return devSetParam(deviceId, &paramDesc, format);
}

enum gnwDeviceError sessionCreate(const procId_t procId, 
                                  const struct gnwDisplayDescriptor * const displayDescriptor, 
                                  const struct session * * sessionPtr) {
    
    CHECKPTR(displayDescriptor);
    CHECKPTR(sessionPtr);

    /*
        Find free session
    */
    struct session * freeSession = nullptr;
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

enum gnwDeviceError sessionEnable(const struct session * const sessionPtr) {

    CHECKPTR(sessionPtr);

    /*
        Set display adapter format
    */

    enum gnwDeviceError e = setDisplayFormat(sessionPtr->displayDescriptor.identifier, 
                                             sessionPtr->displayDescriptor.format);
    if (e) {
        return e;
    }


    /*
        Move process to foreground

        To be implemented
    */

    return GDE_NONE;
}

void sessionDestroy(const struct session * sessionPtr) {
    /*
        TO BE DETERMINED   
    */
    /*
        SHIFT DISPLAY STACK
    */
}
