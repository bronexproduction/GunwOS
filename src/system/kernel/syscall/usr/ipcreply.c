//
//  ipcreply.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.05.2023.
//

#include <hal/proc/proc.h>
#include <ipc/ipc.h>
#include <syscall/func.h>

enum gnwIpcError k_scr_usr_ipcReply(const ptr_t replyBufferPtr, const size_t replySizeBytes, const struct gnwIpcReplyInfo * const infoPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(ptr_t, replyBufferPtr, replySizeBytes, {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS(const struct gnwIpcReplyInfo * const, infoPtr, sizeof(struct gnwIpcReplyInfo), {
        return GIPCE_UNKNOWN;
    });

    return k_ipc_reply(procId, abs_replyBufferPtr, replySizeBytes, abs_infoPtr);
}
