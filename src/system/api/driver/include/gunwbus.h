//
//  gunwbus.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.02.2021.
//

#ifndef GUNWOS_GUNWBUS_H
#define GUNWOS_GUNWBUS_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>

/*
    Read byte from system bus

    Note:
        * Not allowed from user-level
*/
extern uint_8 rdb(const size_t deviceId, const uint_16 port);

/*
    Write byte to system bus

    Note:
        * Not allowed from user-level
*/
extern void wrb(const size_t deviceId, const uint_16 port, uint_8 val);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWBUS_H
