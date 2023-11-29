//
//  session.h
//  GunwOS
//
//  Created by Artur Danielewski on 28.11.2023.
//

#ifndef SESSION_H
#define SESSION_H

#include <gunwdisplay.h>

#define MAX_SESSION 8

struct session {

    /*
        Process ID
    */
    procId_t procId;

    /*
        Display descriptor
    */
    struct gnwDisplayDescriptor displayDescriptor;

};

enum gnwDeviceError sessionCreate(const procId_t procId, 
                                  const struct gnwDisplayDescriptor * const displayDescriptor, 
                                  const struct session * const * sessionPtr);
enum gnwDeviceError sessionEnable(const struct session * const sessionPtr);
struct session * sessionForProc(const procId_t procId);
bool sessionIsOnTop(const struct session * const sessionPtr);
void sessionDestroy(const struct session * sessionPtr);

#endif // SESSION_H
