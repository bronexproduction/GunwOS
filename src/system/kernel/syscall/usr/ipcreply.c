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

    if (!infoPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)infoPtr, sizeof(struct gnwIpcReplyInfo))) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!infoPtr->data.bytes) {
        OOPS("Unexpected reply size", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)infoPtr->data.ptr, infoPtr->data.bytes)) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }

    return k_ipc_reply(procId, infoPtr);
}
