//
//  ipcreply.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.05.2023.
//

#include <ipc/ipc.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcReply(const procId_t procId, const struct gnwIpcReplyInfo * const infoPtr) {

    MEM_VALIDATE_VPTR(procId, infoPtr, struct gnwIpcReplyInfo,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    if (!infoPtr->data.bytes) {
        OOPS("Unexpected reply size", GIPCE_UNKNOWN);
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, infoPtr->data.ptr, infoPtr->data.bytes,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )

    return k_ipc_reply(procId, infoPtr);
}
