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

    // {
    //     char msg[31] = "k_scr_usr_ipcSend - new query ";
    //     LOG(msg);
    // }
    // {
    //     char msg[20] = "  sender -         ";
    //     uint2hex((addr_t)procId, msg + 11);
    //     LOG(msg);
    // }
    // {
    //     char msg[22] = "  receiver -         ";
    //     int2str((addr_t)queryPtr->procId, msg + 13);
    //     LOG(msg);
    // }
    // {
    //     char msg[128] = "  path -         ";
    //     memcopy(queryPtr->pathData.ptr, msg + 9, queryPtr->pathData.bytes);
    //     LOG(msg);
    // }
    // {
    //     char msg[23] = "  query ptr -         ";
    //     uint2hex((addr_t)queryPtr, msg + 14);
    //     LOG(msg);
    // }
    // {
    //     char msg[28] = "  query data ptr -         ";
    //     uint2hex((addr_t)queryPtr->data.ptr, msg + 19);
    //     LOG(msg);
    // }
    // {
    //     char msg[29] = "  query data size -         ";
    //     uint2hex((addr_t)queryPtr->data.bytes, msg + 20);
    //     LOG(msg);
    // }
    // {
    //     char msg[9] = "  bytes:";
    //     LOG_BLOCK(
    //         LOG_NBR(msg);
    //         for (size_t i = 0; i < queryPtr->data.bytes; ++i) {
    //             char byteString[3] = { 0 };
    //             uint2hex((addr_t)queryPtr->data.ptr[i], byteString);
    //             LOG_NBR(" ")
    //             LOG_NBR(byteString);
    //         }
    //     );
    // }
    
    return k_ipc_send(procId, *queryPtr);
}