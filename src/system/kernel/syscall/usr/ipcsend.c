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

    if (!queryPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)queryPtr, sizeof(struct gnwIpcSenderQuery))) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!queryPtr->pathData.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!queryPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)queryPtr->pathData.ptr, queryPtr->pathData.bytes)) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!queryPtr->data.ptr && queryPtr->data.bytes) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (queryPtr->data.ptr && !queryPtr->data.bytes) {
        OOPS("Unexpected data length", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)queryPtr->data.ptr, queryPtr->data.bytes)) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!queryPtr->replyErrPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)queryPtr->replyErrPtr, sizeof(enum gnwIpcError))) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!queryPtr->replyData.ptr && queryPtr->replyData.bytes) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (queryPtr->replyData.ptr && !queryPtr->replyData.bytes) {
        OOPS("Unexpected reply data length", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)queryPtr->replyData.ptr, queryPtr->replyData.bytes)) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    
    return k_ipc_send(procId, *queryPtr);
}