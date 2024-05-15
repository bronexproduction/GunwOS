//
//  ipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef IPC_H
#define IPC_H

#include <src/_gunwipc.h>

void k_ipc_init();

/*
    Handling IPC messages sent by user process

    Available types:
    * Global
    * Direct
*/
enum gnwIpcError k_ipc_send(const procId_t procId,
                            const struct gnwIpcSenderQuery absQuery);

/*
    Handling IPC messages sent by kernel

    Available types:
    * Notification
*/
enum gnwIpcError k_ipc_notify(const struct gnwIpcSenderQuery absQuery,
                              const procId_t target);

enum gnwIpcError k_ipc_register(const procId_t procId, 
                                const data_t absPathData,
                                const gnwIpcListener handlerRoutine,
                                const gnwIpcEndpointQueryDecoder decoder,
                                const bool bindingRequired,
                                const size_t permissionMask);

enum gnwIpcError k_ipc_reply(const procId_t procId,
                             const struct gnwIpcReplyInfo * const absInfoPtr);

void k_ipc_procCleanup(const procId_t procId);

#endif // IPC_H
