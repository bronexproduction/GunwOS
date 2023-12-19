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
    query.path = path;
    query.pathLen = strlen(path);
    query.data = data;
    query.replyErrPtr = &replyErr;
    query.replyData = replyData;
    query.bindData = bindData;

    SYSCALL_PAR1(&query);

    SYSCALL_USER_FUNC(IPC_SEND);
    SYSCALL_USER_INT;

    SYSCALL_GET_RETVAL(32, err);
    return (replyErr == GIPCE_NONE) ? (enum gnwIpcError)err : replyErr;
}

enum gnwIpcError ipcReply(const data_t replyData,
                          const size_t token,
                          const struct gnwIpcBindData bindData) {
    CHECKPTR(replyData.ptr);

    struct gnwIpcReplyInfo info;

    info.data = replyData;
    info.token = token;
    info.bindData = bindData;

    SYSCALL_PAR1(&info);

    SYSCALL_USER_FUNC(IPC_REPLY);
    SYSCALL_USER_INT;

    SYSCALL_RETVAL(32);
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
