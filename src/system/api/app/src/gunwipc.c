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

enum gnwIpcError ipcRegister(const char * const path,
                             const enum gnwIpcAccessScope accessScope,
                             const gnwIpcListener handler) {
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

enum gnwIpcError ipcSend(const char * const path,
                         const ptr_t dataPtr,
                         const size_t dataSizeBytes,
                         const ptr_t replyPtr,
                         const size_t replySizeBytes) {
    CHECKPTR(path);

    struct gnwIpcSenderQuery query;
    query.path = path;
    query.pathLen = strlen(path);
    query.dataPtr = dataPtr;
    query.dataSizeBytes = dataSizeBytes;
    query.replyPtr = replyPtr;
    query.replySizeBytes = replySizeBytes;

    SYSCALL_PAR1(&query);

    SYSCALL_USER_FUNC(IPC_SEND);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwIpcError ipcReply(const ptr_t replyPtr,
                          const size_t replySizeBytes,
                          const size_t token) {
    CHECKPTR(replyPtr);

    SYSCALL_PAR1(replyPtr);
    SYSCALL_PAR2(replySizeBytes);
    SYSCALL_PAR3(token);

    SYSCALL_USER_FUNC(IPC_REPLY);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

void gnwIpcEndpointQuery_decode(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absQueryPtr) {
    memcopy(absDataPtr, absQueryPtr, sizeof(struct gnwIpcEndpointQuery));
    absQueryPtr->dataPtr = absDataPtr + sizeof(struct gnwIpcEndpointQuery);

    #warning result not handled
}

#else

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const absQueryPtr, ptr_t absDataPtr) {
    memcopy(absQueryPtr, absDataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(absQueryPtr->dataPtr, absDataPtr + offset, absQueryPtr->dataSizeBytes);
    offset += absQueryPtr->dataSizeBytes;

    #warning result not handled
}

#endif // _GUNWAPI_KERNEL
