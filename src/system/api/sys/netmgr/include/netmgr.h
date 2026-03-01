//
//  netmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 23.02.2026.
//

#ifndef GUNWOS_API_NETMGR_H
#define GUNWOS_API_NETMGR_H

#include <gunwnetwork.h>
#include <defs.h>

#define NETMGR_PATH_GET "nm/g"

struct gnwNetworkManagerGetSomethingQuery {
    int_32 type;
};

struct gnwNetworkManagerGetSomethingResult {
    int_32 type;
};

#endif // GUNWOS_API_NETMGR_H
