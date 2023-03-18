//
//  gunwrlp.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#ifndef GUNWOS_GUNWRLP_H
#define GUNWOS_GUNWRLP_H

#include <types.h>

/*
    Starting the async run loop

    Needed to be called if the application is expected to use
    asynchronous event listeners
    like devListen()

    Usually needs to be called at the end of the entry point function

    void _start() {
        ...
        ...
        ...
        
        runLoopStart();
    }
*/
void runLoopStart();

/*
    Returns pointer to the main run loop
*/
ptr_t runLoopGetMain();

#endif // GUNWOS_GUNWRLP_H
