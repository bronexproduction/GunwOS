//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <ipc/ipc.h>
#include <hal/mem/mem.h>
#include <defs.h>
#include <error/panic.h>

#include <string.h>
#include <mem.h>
#include <log/log.h>

enum gnwIpcError k_scr_usr_ipcSend(const procId_t procId, const struct gnwIpcSenderQuery * const queryPtr) {

    MEM_VALIDATE_VPTR_R(procId, queryPtr, struct gnwIpcSenderQuery, GIPCE_UNKNOWN);
    MEM_VALIDATE_VPTR_BUFFER(procId, queryPtr->pathData.ptr, queryPtr->pathData.bytes, GIPCE_UNKNOWN, {});
    MEM_VALIDATE_VPTR_BUFFER_NULLABLE(procId, queryPtr->data.ptr, queryPtr->data.bytes, GIPCE_UNKNOWN, {});
    MEM_VALIDATE_VPTR_R(procId, queryPtr->replyErrPtr, enum gnwIpcError, GIPCE_UNKNOWN);
    MEM_VALIDATE_VPTR_BUFFER_NULLABLE(procId, queryPtr->replyData.ptr, queryPtr->replyData.bytes, GIPCE_UNKNOWN, {});
    
    return k_ipc_send(procId, *queryPtr);
}