//
//  gunwctrl.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2021.
//

#ifndef GUNWOS_GUNWCTRL_H
#define GUNWOS_GUNWCTRL_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>
#include <gunwctrltypes.h>

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
