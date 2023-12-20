//
//  ipcreply.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.05.2023.
//

#include <hal/proc/proc.h>
#include <ipc/ipc.h>
#include <syscall/func.h>

enum gnwIpcError k_scr_usr_ipcReply(const struct gnwIpcReplyInfo * const infoPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwIpcReplyInfo * const, infoPtr, sizeof(struct gnwIpcReplyInfo), {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(ptr_t, replyBufferPtr, abs_infoPtr->data.ptr, abs_infoPtr->data.bytes, {
        return GIPCE_UNKNOWN;
    });

    struct gnwIpcReplyInfo absInfo = *abs_infoPtr;
    absInfo.data.ptr = abs_replyBufferPtr;

    return k_ipc_reply(procId, &absInfo);
}
