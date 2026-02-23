//
//  gunwuha_network_desc.h
//  Universal Hardware Abstraction for a network interface card
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#ifndef GUNWOS_GUNWUHA_NETWORK_DESC_H
#define GUNWOS_GUNWUHA_NETWORK_DESC_H

#include <types.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NETWORK_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_network_desc {
    size_t _unused;
};

#endif // GUNWOS_GUNWUHA_NETWORK_DESC_H
