//
//  gunwuha_network.h
//  Universal Hardware Abstraction for a network inteface card
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#ifndef GUNWOS_GUNWUHA_NETWORK_H
#define GUNWOS_GUNWUHA_NETWORK_H

#include <uha/gunwuha_network_desc.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_NETWORK GNW_UHA_EMPTY(NETWORK)
#define GNW_UHA_NETWORK_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_network_routine {
    void (*_unused);
};

struct gnwDeviceUHA_network {
    struct gnwDeviceUHA_network_desc desc;
    struct gnwDeviceUHA_network_routine routine;
};

#endif // GUNWOS_GUNWUHA_NETWORK_H
