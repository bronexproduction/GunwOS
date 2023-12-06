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
#define MAX_DISPLAY 8

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
typedef struct session * sessionPtr_t;

sessionPtr_t sessionForProc(const procId_t procId);
bool sessionIsOnTop(const sessionPtr_t sessionPtr);

enum gnwDeviceError sessionCreate(const procId_t procId, 
                                  const struct gnwDisplayDescriptor * const displayDescriptor, 
                                  sessionPtr_t * const sessionPtr);
enum gnwDeviceError sessionEnable(const sessionPtr_t sessionPtr);
void sessionDestroy(const sessionPtr_t sessionPtr);

#endif // SESSION_H
