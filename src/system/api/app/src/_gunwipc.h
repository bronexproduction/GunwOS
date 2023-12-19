//  _gunwipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.05.2023.
//

#ifndef GUNWOS__GUNWIPC_H
#define GUNWOS__GUNWIPC_H

#include "../include/gunwipc.h"

struct gnwIpcHandlerDescriptor {
    const char * path;
    size_t pathLen;
    gnwIpcListener handlerRoutine;
    gnwIpcEndpointQueryDecoder decoder;
    bool bindingRequired;
    size_t permissionMask;
};

struct gnwIpcSenderQuery {
    procId_t procId;
    const char * path;
    size_t pathLen;
    data_t data;
    enum gnwIpcError * replyErrPtr;
    data_t replyData;
    enum gnwIpcBindFlag bindFlag;
    size_t permissions;
};

struct gnwIpcReplyInfo {
    size_t token;
    enum gnwIpcBindFlag bindFlag;
    size_t permissions;
};

#ifdef _GUNWAPI_KERNEL

void gnwIpcEndpointQuery_encode(const struct gnwIpcEndpointQuery * const, ptr_t);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS__GUNWIPC_H
