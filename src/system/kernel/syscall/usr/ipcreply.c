//
//  ipcreply.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.05.2023.
//

#include <hal/proc/proc.h>
#include <ipc/ipc.h>
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcReply(const struct gnwIpcReplyInfo * const infoPtr) {

    if (!infoPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.bytes) {
        OOPS("Unexpected reply size", GIPCE_UNKNOWN);
    }

    return k_ipc_reply(k_proc_getCurrentId(), infoPtr);
}
