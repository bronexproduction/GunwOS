//
//  utility.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "utility.h"
#include "data.h"
#include <mem.h>
#include <string.h>
#include <gunwipc.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

bool k_ipc_utl_pathIsNotification(const char * absPathPtr, const size_t pathLen) {
    return absPathPtr[0] == GNW_PATH_IPC_BROADCAST_PREFIX;
}

void k_ipc_utl_clearReply(const size_t entryId) {
    if (entryId >= MAX_IPC_TOKEN) {
        OOPS("Reply index out of bounds");
        return;
    }
    
    memzero(&ipcReplyRegister[entryId], sizeof(struct ipcReply));
    ipcReplyRegister[entryId].senderProcId = NONE_PROC_ID;
    ipcReplyRegister[entryId].handlerProcId = NONE_PROC_ID;
}

void k_ipc_utl_unlockIfAble(const procId_t procId) {
    for (size_t token = 0; token < MAX_IPC_TOKEN; ++token) {
        if (ipcReplyRegister[token].senderProcId == procId) {
            return;
        }
    }

    k_proc_unlock(procId, PLT_IPC);
}

size_t k_ipc_utl_nextListenerIndexForPath(const char * absPathPtr, const size_t pathLen, const size_t * const after) {
    for (size_t index = after ? *after + 1 : 0; index < MAX_IPC_LISTENER; ++index) {
        if (!strcmpl(absPathPtr, ipcListenerRegister[index].path, pathLen)) {
            return index;
        }
    }

    return MAX_IPC_LISTENER;
}
