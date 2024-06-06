//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <defs.h>
#include <error/panic.h>

#include <string.h>
#include <mem.h>
#include <log/log.h>

enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr) {

    if (!queryPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!queryPtr->pathData.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!queryPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    if (!queryPtr->data.ptr && queryPtr->data.bytes) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (queryPtr->data.ptr && !queryPtr->data.bytes) {
        OOPS("Unexpected data length", GIPCE_UNKNOWN);
    }
    if (!queryPtr->replyErrPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!queryPtr->replyData.ptr && queryPtr->replyData.bytes) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (queryPtr->replyData.ptr && !queryPtr->replyData.bytes) {
        OOPS("Unexpected reply data length", GIPCE_UNKNOWN);
    }

    {
        char msg[31] = "k_scr_usr_ipcSend - new query ";
        LOG(msg);
    }
    {
        char msg[20] = "  sender -         ";
        uint2hex((addr_t)procId, msg + 11);
        LOG(msg);
    }
    {
        char msg[22] = "  receiver -         ";
        int2str((addr_t)absoluteQuery.procId, msg + 13);
        LOG(msg);
    }
    {
        char msg[128] = "  path -         ";
        memcopy(absoluteQuery.pathData.ptr, msg + 9, absoluteQuery.pathData.bytes);
        LOG(msg);
    }
    {
        char msg[23] = "  query ptr -         ";
        uint2hex((addr_t)queryPtr, msg + 14);
        LOG(msg);
    }
    {
        char msg[28] = "  query data ptr -         ";
        uint2hex((addr_t)absoluteQuery.data.ptr, msg + 19);
        LOG(msg);
    }
    {
        char msg[29] = "  query data size -         ";
        uint2hex((addr_t)absoluteQuery.data.bytes, msg + 20);
        LOG(msg);
    }
    {
        char msg[9] = "  bytes:";
        LOG_BLOCK(
            LOG_NBR(msg);
            for (size_t i = 0; i < queryPtr->data.bytes; ++i) {
                char byteString[3] = { 0 };
                uint2hex((addr_t)queryPtr->data.ptr[i], byteString);
                LOG_NBR(" ")
                LOG_NBR(byteString);
            }
        );
    }
    
    return k_ipc_send(k_proc_getCurrentId(), *queryPtr);
}