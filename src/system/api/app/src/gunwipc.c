//
//  gunwipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#include "_gunwipc.h"
#include <mem.h>

#ifndef _GUNWAPI_KERNEL

#include "scl_user.h"
#include "../include/gunwfug.h"
#include <string.h>

enum gnwIpcError ipcRegister(const char * const path, const enum gnwIpcAccessScope accessScope, const gnwIpcListener handler) {
    CHECKPTR(path);
    CHECKPTR(handler);

    struct gnwIpcHandlerDescriptor desc;
    desc.path = path;
    desc.pathLen = strlen(path);
    if (desc.pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    desc.accessScope = accessScope;
    desc.handlerRoutine = handler;
    desc.decoder = gnwIpcEndpointQuery_decode;

    SYSCALL_PAR1(&desc);

    SYSCALL_USER_FUNC(IPC_REGISTER);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwIpcError ipcSend(const char * const path, const struct gnwIpcQueryParams queryParams) {
    CHECKPTR(path);

    struct gnwIpcSenderQuery query;
    query.path = path;
    query.pathLen = strlen(path);
    query.params = queryParams;

    SYSCALL_PAR1(&query);

    SYSCALL_USER_FUNC(IPC_SEND);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

void gnwIpcEndpointQuery_decode(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absQueryPtr) {
    memcopy(absDataPtr, absQueryPtr, sizeof(struct gnwIpcEndpointQuery));
    absQueryPtr->params.dataPtr = absDataPtr + sizeof(struct gnwIpcEndpointQuery);

    #warning result not handled
}

#else

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const absQueryPtr, ptr_t absDataPtr) {
    memcopy(absQueryPtr, absDataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(absQueryPtr->params.dataPtr, absDataPtr + offset, absQueryPtr->params.dataSizeBytes);
    offset += absQueryPtr->params.dataSizeBytes;

    #warning result not handled
}

#endif // _GUNWAPI_KERNEL
