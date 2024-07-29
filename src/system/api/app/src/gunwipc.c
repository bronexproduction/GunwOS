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

#include "../include/gunwlog.h"

enum gnwIpcError ipcSendDirect(const procId_t procId,
                               const gnwIpcPath path,
                               const data_t data,
                               const data_t replyData,
                               const struct gnwIpcBindData bindData) {
    CHECKPTR(path);

    {
        char msg[27] = "ipcSendDirect - new query ";
        log(msg);
    }
    {
        char msg[22] = "  receiver -         ";
        int2str((addr_t)procId, msg + 13);
        log(msg);
    }
    {
        char msg[128] = "  path -         ";
        memcopy(path, msg + 9, strlen(path));
        log(msg);
    }
    {
        char msg[9] = "  bytes:";
        log(msg);
        for (size_t i = 0; i < data.bytes; ++i) {
            char byteString[3] = { 0 };
            uint2hex((addr_t)data.ptr[i], byteString);
            log(byteString);
        }
    }

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

    enum gnwIpcError error = SYSCALL_RESULT;
    return (replyErr == GIPCE_NONE) ? (enum gnwIpcError)error : replyErr;
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

void gnwIpcEndpointQuery_decode(const ptr_t dataPtr, struct gnwIpcEndpointQuery * const queryPtr) {
    memcopy(dataPtr, queryPtr, sizeof(struct gnwIpcEndpointQuery));
    queryPtr->data.ptr = dataPtr + sizeof(struct gnwIpcEndpointQuery);
}

#else

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const queryPtr, ptr_t dataPtr) {
    memcopy(queryPtr, dataPtr, sizeof(struct gnwIpcEndpointQuery));
    size_t offset = sizeof(struct gnwIpcEndpointQuery);
    memcopy(queryPtr->data.ptr, dataPtr + offset, queryPtr->data.bytes);
    offset += queryPtr->data.bytes;
}

#endif // _GUNWAPI_KERNEL
