//  gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef GUNWOS_GUNWIPC_H
#define GUNWOS_GUNWIPC_H

#include <gunwfug.h>
#include <string.h>

/*
    IPC path structure:

        processId - optional | GNW_ROOT_IPC_PATH_SEPARATOR - only if processId present | pathComponent0 | (GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponentN) - if needed

    Correct path examples:

        * banana
        * banana/potato/strawberry/pizza
        * 0:banana
        * 2:banana/potato
*/ 
#define GNW_ROOT_IPC_PATH_SEPARATOR ":"
#define GNW_PATH_IPC_COMPONENT_SEPARATOR "/"

enum gnwIpcError {
    GIPCE_NONE      = 0,
    GIPCE_UNKNOWN   = -1
};

/*
    Sends given character to specified IPC path
*/
SYSCALL_DECL enum gnwIpcError ipcSend(const char * const path, const char c) {
    CHECKPTR(path);

    SYSCALL_PAR1(path);
    SYSCALL_PAR2(strlen(path));
    SYSCALL_PAR3(c);

    SYSCALL_USER_FUNC(IPC_SEND);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

#endif // GUNWOS_GUNWIPC_H
