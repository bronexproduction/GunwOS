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

enum gnwIpcError ipcRegister(const gnwIpcPath path,
                             const gnwIpcListener handler,
                             const bool bindingRequired,
                             const size_t permissionMask) {
    CHECKPTR(path);
    CHECKPTR(handler);

    struct gnwIpcHandlerDescriptor desc;
    desc.pathData.ptr = (ptr_t)path;
    desc.pathData.bytes = strlen(path);
    if (desc.pathData.bytes > GNW_PATH_IPC_MAX_LENGTH) {
        return GIPCE_INVALID_PATH;
    }
    desc.handlerRoutine = handler;
    desc.decoder = gnwIpcEndpointQuery_decode;
    desc.bindingRequired = bindingRequired;
    desc.permissionMask = permissionMask;

    SYSCALL_USER_CALL(IPC_REGISTER, &desc, 0, 0);

    return SYSCALL_RESULT;
}

enum gnwIpcError ipcRegisterNotification(const gnwIpcPath path,
                                         const gnwIpcListener handler) {
    return ipcRegister(path, handler, false, 0);
}

enum gnwIpcError ipcSend(const gnwIpcPath path,
                         const data_t data,
                         const data_t replyData,
                         const struct gnwIpcBindData bindData) {
    return ipcSendDirect(NONE_PROC_ID, path, data, replyData, bindData);
}

enum gnwIpcError ipcSendDirect(const procId_t procId,
                               const gnwIpcPath path,
                               const data_t data,
                               const data_t replyData,
                               const struct gnwIpcBindData bindData) {
    CHECKPTR(path);

    enum gnwIpcError replyErr = GIPCE_NONE;
    struct gnwIpcSenderQuery query;
    query.procId = procId;
    query.pathData.ptr = (ptr_t)path;
    query.pathData.bytes = strlen(path);
    query.data = data;
    query.replyErrPtr = &replyErr;
    query.replyData = replyData;
    query.bindData = bindData;

    SYSCALL_USER_CALL(IPC_SEND, &query, 0, 0);

    return (replyErr == GIPCE_NONE) ? (enum gnwIpcError)SYSCALL_RESULT : replyErr;
}

enum gnwIpcError ipcReply(const data_t replyData,
                          const size_t token,
                          const struct gnwIpcBindData bindData) {
    CHECKPTR(replyData.ptr);

    struct gnwIpcReplyInfo info;

    info.data = replyData;
    info.token = token;
    info.bindData = bindData;

    SYSCALL_USER_CALL(IPC_REPLY, &info, 0, 0);

    return SYSCALL_RESULT;
}

void gnwIpcEndpointQuery_decode(const ptr_t absDataPtr, struct gnwIpcEndpointQuery * const absQueryPtr) {
    memcopy(absDataPtr, absQueryPtr, sizeof(struct gnwIpcEndpointQuery));
    absQueryPtr->data.ptr = absDataPtr + sizeof(struct gnwIpcEndpointQuery);
}

#else

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const absQueryPtr, ptr_t absDataPtr) {
    memcopy(absQueryPtr, absDataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(absQueryPtr->data.ptr, absDataPtr + offset, absQueryPtr->data.bytes);
    offset += absQueryPtr->data.bytes;
}

#endif // _GUNWAPI_KERNEL
