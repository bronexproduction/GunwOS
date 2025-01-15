//
//  _gunwdrv.h
//  GunwOS
//
//  Created by Artur Danielewski on 12.04.2023.
//

#ifndef GUNWOS__GUNWDRV_H
#define GUNWOS__GUNWDRV_H

#ifdef _GUNWAPI_KERNEL

#include "../include/gunwdrv.h"
#include <_gunwdev.h>

#define GNW_DEVICEGETPARAMQUERY_ENCODEDSIZE (sizeof(struct gnwDeviceGetParamQuery))
#define GNW_DEVICESETPARAMQUERY_ENCODEDSIZE (sizeof(struct gnwDeviceSetParamQuery))
#define GNW_DEVICEMEMWRITEQUERY_ENCODEDSIZE(QUERY) (sizeof(struct gnwDeviceMemWriteQuery) + QUERY.inputBufferRange.sizeBytes)

bool validateDeviceDescriptor(const struct gnwDeviceDescriptor * const);

void gnwDeviceGetParamQuery_encode(const struct gnwDeviceGetParamQuery * const, ptr_t);
void gnwDeviceSetParamQuery_encode(const struct gnwDeviceSetParamQuery * const, ptr_t);
void gnwDeviceMemWriteQuery_encode(const struct gnwDeviceMemWriteQuery * const, ptr_t);

#endif // GUNWOS__GUNWDRV_H

#endif // _GUNWAPI_KERNEL
