//  gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef GUNWOS_GUNWIPC_H
#define GUNWOS_GUNWIPC_H

#include <types.h>

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

struct gnwIpcSenderQuery {
    const char * path;
    size_t pathLen;
    ptr_t dataPtr;
    size_t dataSizeBytes;
    ptr_t resultPtr;
    size_t resultSizeBytes;
};

struct gnwIpcEndpointQuery {
    procId_t sourceProcId;
    ptr_t dataPtr;
    size_t dataSizeBytes;
    size_t resultSizeBytes;
};

typedef void (*gnwIpcListener)(const struct gnwIpcEndpointQuery * const);

typedef void (*gnwIpcEndpointQueryDecoder)(const ptr_t, struct gnwIpcEndpointQuery * const);

struct gnwIpcHandlerDescriptor {
    const char * path;
    size_t pathLen;
    enum gnwIpcAccessScope accessScope;
    gnwIpcListener handlerRoutine;
    gnwIpcEndpointQueryDecoder decoder;
};

#ifndef _GUNWAPI_KERNEL

extern void gnwIpcEndpointQuery_decode(const ptr_t, struct gnwIpcEndpointQuery * const);

/*
    Registers the process as global IPC receiver for given path

    Params:
        * path - IPC path (see line 14)
        * accessScope - IPC path access scope
        * handler - IPC message handler
*/
extern enum gnwIpcError ipcRegister(const char * const path,
                                    const enum gnwIpcAccessScope accessScope,
                                    const gnwIpcListener handler);

/*
    Sends query to specified IPC path

    Params:
        * path - IPC path (see line 14)
        * dataPtr - pointer to message data
        * dataSizeBytes - size of the message in bytes
        * resultPtr - address of the buffer for result data
        * resultSizeBytes - size of the result in bytes
*/
extern enum gnwIpcError ipcSend(const char * const path,
                                const ptr_t dataPtr,
                                const size_t dataSizeBytes,
                                const ptr_t resultPtr,
                                const size_t resultSizeBytes);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWIPC_H
