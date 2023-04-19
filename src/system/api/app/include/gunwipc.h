//  gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef GUNWOS_GUNWIPC_H
#define GUNWOS_GUNWIPC_H

#include <gunwfug.h>
#include <gunwrlp.h>
#include <gunwevent.h>
#include <string.h>

/*
    IPC path structure:

        processId - optional | GNW_ROOT_IPC_PATH_SEPARATOR - only if processId present | pathComponent0 | (GNW_PATH_IPC_COMPONENT_SEPARATOR | pathComponentN) - if needed

    Correct path examples:

        * banana
        * banana/potato/strawberry/pizza
        * 0:banana
        * 2:banana/potato
    
    Allowed characters: a-z A-Z 0-9
*/ 
#define GNW_ROOT_IPC_PATH_SEPARATOR ":"
#define GNW_PATH_IPC_COMPONENT_SEPARATOR "/"
#define GNW_PATH_IPC_MAX_LENGTH 64

enum gnwIpcError {
    GIPCE_NONE              = 0,
    GIPCE_INVALID_PATH      = -2,
    GIPCE_INVALID_PARAMETER = -3,
    GIPCE_ALREADY_EXISTS    = -4,
    GIPCE_NOT_FOUND         = -5,
    GIPCE_IGNORED           = -6,
    GIPCE_FORBIDDEN         = -7,
    GIPCE_UNKNOWN           = -1
};

enum gnwIpcAccessScope {
    GIAS_NONE       = 0,
    GIAS_KERNEL     = (1 << 0),
    GIAS_USER       = (1 << 1),
    GIAS_ALL        = GIAS_KERNEL | GIAS_USER
};

struct gnwIpcHandlerDescriptor {
    const char * path;
    size_t pathLen;
    enum gnwIpcAccessScope accessScope;
    gnwEventListener_32_8 handlerRoutine;
};

/*
    Registers the process as global IPC receiver for given path

    Params:
        * path - IPC path (see line 14)
        * accessScope - IPC path access scope
        * handler - IPC message handler
*/
SYSCALL_DECL enum gnwIpcError ipcRegister(const char * const path, const enum gnwIpcAccessScope accessScope, const gnwEventListener_32_8 handler) {
    CHECKPTR(path);
    CHECKPTR(handler);

    ptr_t rlpPtr = runLoopGetMain();
    CHECKPTR(rlpPtr);

    struct gnwIpcHandlerDescriptor desc;
    desc.path = path;
    desc.pathLen = strlen(path);
    desc.accessScope = accessScope;
    desc.handlerRoutine = handler;

    SYSCALL_PAR1(&desc);
    SYSCALL_PAR2(rlpPtr);

    SYSCALL_USER_FUNC(IPC_REGISTER);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

/*
    Sends given character to specified IPC path

    Params:
        * path - IPC path (see line 14)
        * c - character to be sent
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
