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
#include <defs.h>
#include <string.h>
#include <proc.h>

gnwIpcListener ipcSessionDestroyListener = nullptr;

enum gnwIpcError ipcRegister(const char * const path,
                             const gnwIpcListener handler,
                             const bool bindingRequired,
                             const size_t permissionMask) {
    CHECKPTR(path);
    CHECKPTR(handler);

    struct gnwIpcHandlerDescriptor desc;
    desc.path = path;
    desc.pathLen = strlen(path);
    if (desc.pathLen > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    desc.handlerRoutine = handler;
    desc.decoder = gnwIpcEndpointQuery_decode;
    desc.bindingRequired = bindingRequired;
    desc.permissionMask = permissionMask;

    SYSCALL_PAR1(&desc);

    SYSCALL_USER_FUNC(IPC_REGISTER);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

enum gnwIpcError ipcRegisterNotification(const char * const path,
                                         const gnwIpcListener handler) {
    return ipcRegister(path, handler, false, 0);
}

enum gnwIpcError ipcSend(const char * const path,
                         const ptr_t dataPtr,
                         const size_t dataSizeBytes,
                         ptr_t replyPtr,
                         const size_t replySizeBytes,
                         const enum gnwIpcBindFlag bindFlag,
                         const size_t permissions) {
    return ipcSendDirect(NONE_PROC_ID, path, dataPtr, dataSizeBytes, replyPtr, replySizeBytes, bindFlag, permissions);
}

enum gnwIpcError ipcSendDirect(const procId_t procId,
                               const char * const path,
                               const ptr_t dataPtr,
                               const size_t dataSizeBytes,
                               ptr_t replyPtr,
                               const size_t replySizeBytes,
                               const enum gnwIpcBindFlag bindFlag,
                               const size_t permissions) {
    CHECKPTR(path);

    enum gnwIpcError replyErr = GIPCE_NONE;
    struct gnwIpcSenderQuery query;
    query.procId = procId;
    query.path = path;
    query.pathLen = strlen(path);
    query.dataPtr = dataPtr;
    query.dataSizeBytes = dataSizeBytes;
    query.replyErrPtr = &replyErr;
    query.replyPtr = replyPtr;
    query.replySizeBytes = replySizeBytes;
    query.bindFlag = bindFlag;
    query.permissions = permissions;

    SYSCALL_PAR1(&query);

    SYSCALL_USER_FUNC(IPC_SEND);
    SYSCALL_USER_INT;

    SYSCALL_GET_RETVAL(32, err);
    return (replyErr == GIPCE_NONE) ? (enum gnwIpcError)err : replyErr;
}

enum gnwIpcError ipcReply(const ptr_t replyPtr,
                          const size_t replySizeBytes,
                          const size_t token,
                          const enum gnwIpcBindFlag bindFlag,
                          const size_t permissions) {
    CHECKPTR(replyPtr);

    struct gnwIpcReplyInfo info;

    info.token = token;
    info.bindFlag = bindFlag;
    info.permissions = permissions;

    SYSCALL_PAR1(replyPtr);
    SYSCALL_PAR2(replySizeBytes);
    SYSCALL_PAR3(&info);

    SYSCALL_USER_FUNC(IPC_REPLY);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
}

void gnwIpcEndpointQuery_decode(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absQueryPtr) {
    memcopy(absDataPtr, absQueryPtr, sizeof(struct gnwIpcEndpointQuery));
    absQueryPtr->dataPtr = absDataPtr + sizeof(struct gnwIpcEndpointQuery);
}

#else

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const absQueryPtr, ptr_t absDataPtr) {
    memcopy(absQueryPtr, absDataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(absQueryPtr->dataPtr, absDataPtr + offset, absQueryPtr->dataSizeBytes);
    offset += absQueryPtr->dataSizeBytes;
}

#endif // _GUNWAPI_KERNEL
