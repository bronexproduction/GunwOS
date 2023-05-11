//
//  scl_user.h
//  GunwOS
//
//  Created by Artur Danielewski on 19.04.2023.
//

#ifndef GUNWOS_SCL_USER_H
#define GUNWOS_SCL_USER_H

#warning commented until drivers running in separate processes
// #ifndef _GUNWAPI_KERNEL

#include <scl_def.h>

#define SYSCALL_USER_INTERRUPT                  0x69

#define SYSCALL_USER_FUNCTION_START             0x00
/* 0x01 */
#define SYSCALL_USER_FUNCTION_DEV_CHAR_WRITE    0x02
#define SYSCALL_USER_FUNCTION_EXIT              0x03
#define SYSCALL_USER_FUNCTION_WAIT_FOR_EVENT    0x04
#define SYSCALL_USER_FUNCTION_TIME_MS           0x05
#define SYSCALL_USER_FUNCTION_IPC_SEND          0x06
#define SYSCALL_USER_FUNCTION_IPC_REGISTER      0x07
#define SYSCALL_USER_FUNCTION_DEV_GET_BY_ID     0x08
#define SYSCALL_USER_FUNCTION_DEV_GET_BY_TYPE   0x09
#define SYSCALL_USER_FUNCTION_DEV_ACQUIRE       0x0a
#define SYSCALL_USER_FUNCTION_DEV_RELEASE       0x0b
#define SYSCALL_USER_FUNCTION_DEV_MEM_WRITE     0x0c
#define SYSCALL_USER_FUNCTION_FUG               0x0d
#define SYSCALL_USER_FUNCTION_DEV_LISTEN        0x0e
#define SYSCALL_USER_FUNCTION_DEV_GET_PARAM     0x0f
#define SYSCALL_USER_FUNCTION_DEV_SET_PARAM     0x10
#define SYSCALL_USER_FUNCTION_RUNLOOP_GET_ITEM  0x11
#define SYSCALL_USER_FUNCTION_RUNLOOP_GET_DATA  0x12

#define SYSCALL_USER_FUNC(CODE) SYSCALL_FUNC(USER, CODE)
#define SYSCALL_USER_INT SYSCALL_INT(SYSCALL_USER_INTERRUPT)

#warning commented until drivers running in separate processes
// #endif // _GUNWAPI_KERNEL

#endif // GUNWOS_SCL_USER_H
