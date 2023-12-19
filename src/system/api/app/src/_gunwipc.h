//  _gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifndef GUNWOS__GUNWIPC_H
#define GUNWOS__GUNWIPC_H

#include "../include/gunwipc.h"

struct gnwIpcHandlerDescriptor {
    gnwIpcPath path;
    size_t pathLen;
    gnwIpcListener handlerRoutine;
    gnwIpcEndpointQueryDecoder decoder;
    bool bindingRequired;
    size_t permissionMask;
};

struct gnwIpcSenderQuery {
    procId_t procId;
    gnwIpcPath path;
    size_t pathLen;
    data_t data;
    enum gnwIpcError * replyErrPtr;
    data_t replyData;
    struct gnwIpcBindData bindData;
};

struct gnwIpcReplyInfo {
    data_t data;
    size_t token;
    struct gnwIpcBindData bindData;
};

#ifdef _GUNWAPI_KERNEL

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const, ptr_t);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS__GUNWIPC_H
