//  _gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifdef _GUNWAPI_KERNEL

#ifndef GUNWOS__GUNWIPC_H
#define GUNWOS__GUNWIPC_H

#include <gunwipc.h>

void gnwIpcEndpointQuery_serialize(const struct gnwIpcEndpointQuery * const, ptr_t);
void gnwIpcEndpointQuery_deserialize(const ptr_t, struct gnwIpcEndpointQuery * const);

#endif // GUNWOS_GUNWIPC_H

#endif // _GUNWAPI_KERNEL