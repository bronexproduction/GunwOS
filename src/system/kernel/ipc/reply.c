//
//  reply.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "ipc.h"
#include "data.h"
#include "binding.h"
#include "utility.h"
#include <src/_gunwipc.h>
#include <mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

enum gnwIpcError k_ipc_reply(const procId_t procId,
                             const ptr_t absReplyBufferPtr,
                             const size_t replySizeBytes,
                             const struct gnwIpcReplyInfo * const absInfoPtr) {
    if (!absReplyBufferPtr) {
        OOPS("Null reply buffer pointer");
        return GIPCE_UNKNOWN;
    }
    if (!absInfoPtr) {
        OOPS("Null reply info pointer");
        return GIPCE_UNKNOWN;
    }
    if (!replySizeBytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (absInfoPtr->token >= MAX_IPC_TOKEN) {
        return GIPCE_INVALID_PARAMETER;
    }

    const struct ipcReply * const reply = &ipcReplyRegister[absInfoPtr->token];
    if (reply->handlerProcId == NONE_PROC_ID) {
        /*
            Empty slot

            Note: Can happen if sender process terminated before receiving the reply
        */
        return GIPCE_NOT_FOUND;
    }
    if (reply->handlerProcId != procId) {
        return GIPCE_FORBIDDEN;
    }
    if (reply->replySizeBytes != replySizeBytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!reply->absReplyErrorPtr) {
        OOPS("Unexpected reply error nullptr");
        return GIPCE_UNKNOWN;
    }
    if (!reply->absReplyBufferPtr) {
        OOPS("Unexpected reply buffer nullptr");
        return GIPCE_UNKNOWN;
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply");
        return GIPCE_UNKNOWN;
    }

    enum gnwIpcError e = GIPCE_NONE;
    switch (absInfoPtr->bindFlag) {
        case GIBF_BIND:
            e = k_ipc_binding_create(senderProcId, procId, absInfoPtr->permissions);
            if (e == GIPCE_ALREADY_EXISTS) {
                e = GIPCE_NONE;
            }
            break;
        case GIBF_UPDATE:
            e = k_ipc_binding_update(senderProcId, procId, absInfoPtr->permissions);
            break;
        case GIBF_UNBIND:
            e = k_ipc_binding_destroy(senderProcId, procId, procId);
            break;
        default:
            break;
    }
    if (e != GIPCE_NONE) {
        return e;
    }

    memcopy(absReplyBufferPtr, reply->absReplyBufferPtr, replySizeBytes);
    *(reply->absReplyErrorPtr) = GIPCE_NONE;

    k_ipc_utl_clearReply(absInfoPtr->token);
    k_ipc_utl_unlockIfAble(senderProcId);

    return GIPCE_NONE;
}
