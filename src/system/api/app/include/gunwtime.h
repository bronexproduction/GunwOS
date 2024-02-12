//
//  gunwtime.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2023.
//

#ifndef GUNWOS_GUNWTIME_H
#define GUNWOS_GUNWTIME_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>

/*
    Returns kernel time in milliseconds
*/
extern time_t timeMs();

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWTIME_H
