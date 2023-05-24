//
//  ipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef IPC_H
#define IPC_H

#include <gunwipc.h>

enum gnwIpcError k_ipc_send(const procId_t procId,
                            const struct gnwIpcSenderQuery absQuery);
enum gnwIpcError k_ipc_register(const procId_t procId, 
                                const char * const absPathPtr,
                                const size_t pathLen,
                                const enum gnwIpcAccessScope accessScope,
                                const gnwIpcListener handlerRoutine,
                                const gnwIpcEndpointQueryDecoder decoder);
enum gnwIpcError k_ipc_reply(const procId_t procId,
                             const ptr_t absReplyBufferPtr,
                             const size_t replySizeBytes,
                             const size_t token);

void k_ipc_procCleanup(const procId_t procId);

#endif // IPC_H
