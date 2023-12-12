//
//  ipc.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include "ipc.h"
#include "binding.h"
#include "data.h"
#include "utility.h"
#include <mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

static void clearListener(const size_t entryId) {
    if (entryId >= MAX_IPC_LISTENER) {
        OOPS("Listener index out of bounds");
        return;
    }
    
    memzero(&ipcListenerRegister[entryId], sizeof(struct ipcListener));
    ipcListenerRegister[entryId].procId = NONE_PROC_ID;
}

void k_ipc_init() {
    for (size_t i = 0; i < MAX_IPC_LISTENER; clearListener(i++));
    for (size_t i = 0; i < MAX_IPC_TOKEN; k_ipc_utl_clearReply(i++));
    k_ipc_binding_initialize();
}

static void failReply(const size_t token) {
    if (token >= MAX_IPC_TOKEN) {
        OOPS("Unexpected IPC token on error");
        return;
    }

    const struct ipcReply * const reply = &ipcReplyRegister[token];
    if (reply->handlerProcId == NONE_PROC_ID) {
        OOPS("Unexpected empty IPC slot on error");
        return;
    }
    if (!reply->absReplyErrorPtr) {
        OOPS("Unexpected reply error nullptr");
        return;
    }
    const procId_t senderProcId = reply->senderProcId;
    if (senderProcId <= KERNEL_PROC_ID) {
        OOPS("Unexpected sender process ID on IPC reply");
        return;
    }

    *(reply->absReplyErrorPtr) = GIPCE_IGNORED;

    k_ipc_utl_clearReply(token);
    k_ipc_utl_unlockIfAble(senderProcId);
}

void k_ipc_procCleanup(const procId_t procId) {
    for (size_t entryId = 0; entryId < MAX_IPC_LISTENER; ++entryId) {
        if (ipcListenerRegister[entryId].procId == procId) {
            clearListener(entryId);
        }
    }
    for (int token = 0; token < MAX_IPC_TOKEN; ++token) {
        if (ipcReplyRegister[token].handlerProcId == procId) {
            /*
                Handler process terminated

                Note: Nullptr reply must be issued and sender process IPC lock must be released
            */
            failReply(token);
        } else if (ipcReplyRegister[token].senderProcId == procId) {
            /*
                Sender process terminated

                Note: Handler process will get GIPCE_NOT_FOUND as ipcReply return value for current token
            */
            k_ipc_utl_clearReply(token);
        }
    }
    k_ipc_binding_cleanup(procId);
}