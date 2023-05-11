//
//  scl_driver.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_SCL_DRIVER_H
#define GUNWOS_SCL_DRIVER_H

#include <scl_def.h>

#define SYSCALL_DRIVER_INTERRUPT                0x45

#define SYSCALL_DRIVER_FUNCTION_RDB             0x00
#define SYSCALL_DRIVER_FUNCTION_WRB             0x01
#define SYSCALL_DRIVER_FUNCTION_EMIT            0x02

#define SYSCALL_DRIVER_FUNC(CODE) SYSCALL_FUNC(DRIVER, CODE)
#define SYSCALL_DRIVER_INT SYSCALL_INT(SYSCALL_DRIVER_INTERRUPT)

#endif // GUNWOS_SCL_DRIVER_H
