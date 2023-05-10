//  _gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifndef GUNWOS__GUNWIPC_H
#define GUNWOS__GUNWIPC_H

#include <gunwipc.h>

#ifdef _GUNWAPI_KERNEL

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const, ptr_t);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS__GUNWIPC_H