//
//  data.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#ifndef DATA_H
#define DATA_H

#include <gunwipc.h>

#define MAX_IPC_LISTENER 16
#define MAX_IPC_TOKEN 8

enum gnwIpcListenerType {
    GILT_NONE         = 0,
    GILT_GLOBAL       = (1 << 0),
    GILT_DIRECT       = (1 << 1),
    GILT_NOTIFICATION = (1 << 2)
};

struct ipcListener {
    procId_t procId;
    char path[GNW_PATH_IPC_MAX_LENGTH];
    enum gnwIpcListenerType type;
    gnwIpcListener listener;
    gnwIpcEndpointQueryDecoder decoder;
    bool bindingRequired;
    size_t permissionMask;
};

struct ipcReply {
    procId_t senderProcId;
    procId_t handlerProcId;
    enum gnwIpcError * absReplyErrorPtr;
    ptr_t absReplyBufferPtr;
    size_t replySizeBytes;
};

struct ipcListener ipcListenerRegister[MAX_IPC_LISTENER];
struct ipcReply ipcReplyRegister[MAX_IPC_TOKEN];

#endif // DATA_H
