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
#include <_gunwipc.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <hal/paging/paging.h>
#include <error/panic.h>

enum gnwIpcError k_ipc_reply(const procId_t procId,
                             const struct gnwIpcReplyInfo * const infoPtr) {
    if (!infoPtr) {
        OOPS("Null reply info pointer", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.ptr) {
        OOPS("Null reply buffer pointer", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.bytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (infoPtr->token >= MAX_IPC_TOKEN) {
        return GIPCE_INVALID_PARAMETER;
    }

    const struct ipcReply * const reply = &ipcReplyRegister[infoPtr->token];
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
    if (reply->replyBufferData.bytes != infoPtr->data.bytes) {
        return GIPCE_INVALID_PARAMETER;
    }
    if (!reply->replyErrorPtr) {
        OOPS("Unexpected reply error nullptr", GIPCE_UNKNOWN);
    }
    if (!reply->replyBufferData.ptr) {
        OOPS("Unexpected reply buffer nullptr", GIPCE_UNKNOWN);
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply", GIPCE_UNKNOWN);
    }

    enum gnwIpcError e = GIPCE_NONE;
    switch (infoPtr->bindData.flag) {
        case GIBF_BIND:
            e = k_ipc_binding_create(senderProcId, procId, infoPtr->bindData.permissions);
            if (e == GIPCE_ALREADY_EXISTS) {
                e = GIPCE_NONE;
            }
            break;
        case GIBF_UPDATE:
            e = k_ipc_binding_update(senderProcId, procId, infoPtr->bindData.permissions);
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

    k_mem_copy(procId, infoPtr->data.ptr,
               senderProcId, reply->replyBufferData.ptr,
               infoPtr->data.bytes);
    
    MEM_ONTABLE(senderProcId, {
        *(reply->replyErrorPtr) = GIPCE_NONE;
    });

    k_ipc_utl_clearReply(infoPtr->token);
    k_ipc_utl_unlockIfAble(senderProcId);

    return GIPCE_NONE;
}
