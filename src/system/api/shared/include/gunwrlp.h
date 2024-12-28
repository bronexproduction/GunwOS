//
//  gunwrlp.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS_GUNWRLP_H
#define GUNWOS_GUNWRLP_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>

/*
    Starting the async run loop

    Needed to be called if the application is expected to use
    asynchronous event listeners
    like devListen() or IPC messaging

    Usually needs to be called at the end of the entry point function

    void dupa() {
        ...
        ...
        ...
        
        runLoopStart();
    }
*/
void runLoopStart();

/*
    Synchronously forcing the run loop
    to process all the current events
*/
void runLoopHandle();

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWRLP_H
