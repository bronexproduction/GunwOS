//
//  session.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef SESSION_H
#define SESSION_H

#include <types.h>

#define MAX_SESSION 8

struct session {

    /*
        Process ID
    */
    procId_t procId;

};
typedef struct session * sessionPtr_t;

sessionPtr_t sessionForProc(const procId_t procId);
struct session * sessionOnTop();
bool sessionIsOnTop(const sessionPtr_t sessionPtr);

enum gnwDeviceError sessionCreate(const procId_t procId,
                                  sessionPtr_t * const sessionPtr);
enum gnwDeviceError sessionEnable(const sessionPtr_t sessionPtr);
void sessionDestroy(const sessionPtr_t sessionPtr);

#endif // SESSION_H
