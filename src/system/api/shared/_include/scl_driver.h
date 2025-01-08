//
//  scl_driver.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_SCL_DRIVER_H
#define GUNWOS_SCL_DRIVER_H

#ifndef _GUNWAPI_KERNEL

#include "scl_def.h"

#define SYSCALL_DRIVER_INTERRUPT                0x45

#define SYSCALL_DRIVER_FUNCTION_RDB             0x00
#define SYSCALL_DRIVER_FUNCTION_WRB             0x01
#define SYSCALL_DRIVER_FUNCTION_EMIT            0x02
#define SYSCALL_DRIVER_FUNCTION_MMIO_PLZ        0x03

#define SYSCALL_DRIVER_CALL(CODE, PAR1, PAR2, PAR3) \
SYSCALL_CALL(SYSCALL_DRIVER_INTERRUPT, SYSCALL_DRIVER_FUNCTION_ ## CODE, PAR1, PAR2, PAR3)

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_SCL_DRIVER_H
