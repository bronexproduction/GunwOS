//
//  gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_GUNWCTRL_H
#define GUNWOS_GUNWCTRL_H

#include <types.h>

enum gnwCtrlError {
    GCE_NONE = 0,
    GCE_INVALID_ARGUMENT,
    GCE_HEADER_INVALID,
    GCE_NOT_FOUND,
    GCE_OPERATION_FAILED,
    GCE_UNKNOWN
};

#ifndef _GUNWAPI_KERNEL

/*
    Starts program

    Params:
    * path - path to the executable
    * error - result error pointer
*/
extern void start(const char * const path, enum gnwCtrlError * const error);

/*
    Ends process execution
*/
extern void bye(const int_32 status);

/*
    Suspends process execution until an event is received
*/
extern void waitForEvent();

/*
    Ceases to claim associated CPU time; 
    frees up the CPU in order to let other processes claim its time slice
*/
extern void yield();

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWCTRL_H
