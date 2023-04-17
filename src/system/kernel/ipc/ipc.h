//
//  ipc.h
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#ifndef IPC_H
#define IPC_H

#include <gunwipc.h>
#include <src/_gunwrlp.h>

enum gnwIpcError k_ipc_ipcSend(const procId_t procId, 
                               const char * const absPathPtr,
                               const size_t pathLen, 
                               const char c);

enum gnwIpcError k_ipc_ipcRegister(const procId_t procId, 
                                   const char * const absPathPtr,
                                   const size_t pathLen,
                                   const enum gnwIpcAccessScope accessScope,
                                   const gnwEventListener_32_8 handlerRoutine,
                                   struct gnwRunLoop * const runLoopPtr);

void k_ipc_procCleanup(const procId_t procId);

#endif // IPC_H
