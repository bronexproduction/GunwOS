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
                             const struct gnwIpcReplyInfo * const absInfoPtr) {
    if (!absInfoPtr) {
        OOPS("Null reply info pointer", GIPCE_UNKNOWN);
    }
    if (!absInfoPtr->data.ptr) {
        OOPS("Null reply buffer pointer", GIPCE_UNKNOWN);
    }
    if (!absInfoPtr->data.bytes) {
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
    if (reply->absReplyBufferData.bytes != absInfoPtr->data.bytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!reply->absReplyErrorPtr) {
        OOPS("Unexpected reply error nullptr", GIPCE_UNKNOWN);
    }
    if (!reply->absReplyBufferData.ptr) {
        OOPS("Unexpected reply buffer nullptr", GIPCE_UNKNOWN);
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply", GIPCE_UNKNOWN);
    }

    enum gnwIpcError e = GIPCE_NONE;
    switch (absInfoPtr->bindData.flag) {
        case GIBF_BIND:
            e = k_ipc_binding_create(senderProcId, procId, absInfoPtr->bindData.permissions);
            if (e == GIPCE_ALREADY_EXISTS) {
                e = GIPCE_NONE;
            }
            break;
        case GIBF_UPDATE:
            e = k_ipc_binding_update(senderProcId, procId, absInfoPtr->bindData.permissions);
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

    memcopy(absInfoPtr->data.ptr, reply->absReplyBufferData.ptr, absInfoPtr->data.bytes);
    *(reply->absReplyErrorPtr) = GIPCE_NONE;

    k_ipc_utl_clearReply(absInfoPtr->token);
    k_ipc_utl_unlockIfAble(senderProcId);

    return GIPCE_NONE;
}
