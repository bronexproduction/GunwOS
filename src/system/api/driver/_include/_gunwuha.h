//
//  _gunwuha.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2025.
//

#ifndef GUNWOS__GUNWUHA_H
#define GUNWOS__GUNWUHA_H

#include <gunwdevtypes.h>

#ifdef _GUNWAPI_KERNEL

/*
    Extracts UHA descriptor from UHA structure
*/
extern struct gnwDeviceUHADesc uhaGetDesc(const size_t identifier,
                                          const enum gnwDeviceType type,
                                          const struct gnwDeviceUHA api);

#define GNW_DEVICEGETPARAMQUERY_ENCODEDSIZE (sizeof(struct gnwDeviceGetParamQuery))
#define GNW_DEVICESETPARAMQUERY_ENCODEDSIZE (sizeof(struct gnwDeviceSetParamQuery))
#define GNW_DEVICEMEMWRITEQUERY_ENCODEDSIZE(QUERY) (sizeof(struct gnwDeviceMemWriteQuery) + QUERY.inputBufferRange.sizeBytes)

void gnwDeviceGetParamQuery_encode(const struct gnwDeviceGetParamQuery * const, ptr_t);
void gnwDeviceSetParamQuery_encode(const struct gnwDeviceSetParamQuery * const, ptr_t);
void gnwDeviceMemWriteQuery_encode(const struct gnwDeviceMemWriteQuery * const, ptr_t);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS__GUNWUHA_H
