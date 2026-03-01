//
//  gunwnetwork.h
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#ifndef GUNWOS_GUNWNETWORK_H
#define GUNWOS_GUNWNETWORK_H

#ifndef _GUNWAPI_KERNEL

#include <types.h>
#include "uha/gunwuha_network_desc.h"

typedef struct {
    byte_t bytes[6];
} gnwNetworkMacAddr;

enum gnwNetworkError {
    GNE_NONE
};

/*
    Physical interface properties
*/
enum gnwNetworkError getMac(gnwNetworkMacAddr * const result);

/*
    Internet Control Message Protocol
*/
enum gnwNetworkError icmpEcho();

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWNETWORK_H
