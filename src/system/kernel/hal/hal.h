//
//  hal.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef HAL_H
#define HAL_H

#include <types.h>
#include <gunwdrv.h>

enum gnwDriverError k_hal_install(const size_t devId, const struct gnwDriverConfig driver);

extern const size_t *k_hal_servicedDevIdPtr;

#endif // HAL_H
