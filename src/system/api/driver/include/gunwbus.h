//
//  gunwbus.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.02.2021.
//

#ifndef GUNWOS_GUNWBUS_H
#define GUNWOS_GUNWBUS_H

#warning commented until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL

#include <types.h>

/*
    Read byte from system bus

    Note:
        * Not allowed from user-level
*/
extern uint_8 rdb(uint_16 const port);

/*
    Write byte to system bus

    Note:
        * Not allowed from user-level
*/
extern void wrb(uint_16 const port, uint_8 const val);

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWBUS_H
