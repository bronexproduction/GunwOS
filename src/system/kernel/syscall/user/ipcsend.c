//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <ipc/ipc.h>
#include <hal/mem/mem.h>
#include <defs.h>
#include <error/panic.h>

#include <string.h>
#include <mem.h>
#include <log/log.h>

enum gnwIpcError k_scr_usr_ipcSend(const procId_t procId, const struct gnwIpcSenderQuery * const queryPtr) {

    MEM_VALIDATE_VPTR(procId, queryPtr, struct gnwIpcSenderQuery,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    if (!queryPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, queryPtr->pathData.ptr, queryPtr->pathData.bytes,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    if (!queryPtr->data.bytes) {
        OOPS("Unexpected data length", GIPCE_UNKNOWN);
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, queryPtr->data.ptr, queryPtr->data.bytes,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    MEM_VALIDATE_VPTR(procId, queryPtr->replyErrPtr, enum gnwIpcError,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    if (!queryPtr->replyData.bytes) {
        OOPS("Unexpected reply data length", GIPCE_UNKNOWN);
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, queryPtr->replyData.ptr, queryPtr->replyData.bytes,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    
    return k_ipc_send(procId, *queryPtr);
}