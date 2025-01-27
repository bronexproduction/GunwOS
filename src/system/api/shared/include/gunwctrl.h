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
#include "gunwctrltypes.h"

/*
    Starts program

    Params:
    * path - path to the executable
    * procIdResultPtr - optional pointer where started process identifier will be stored
    
    Returns:
    * start error (gnwCtrlError) if any, otherwise GCE_NONE
*/
extern enum gnwCtrlError start(const char * const path, procId_t * const procIdResultPtr);

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
