//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <syscall/func.h>
#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <defs.h>

#include <string.h>
#include <mem.h>
#include <log/log.h>

enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwIpcSenderQuery * const, queryPtr, sizeof(struct gnwIpcSenderQuery), {}, GIPCE_UNKNOWN);
    SCLF_GET_VALID_ABS_NAMED(const gnwIpcPath, pathPtr, abs_queryPtr->pathData.ptr, abs_queryPtr->pathData.bytes, {}, GIPCE_UNKNOWN);
    SCLF_GET_VALID_NULLABLE_ABS_NAMED(const ptr_t, dataPtr, abs_queryPtr->data.ptr, abs_queryPtr->data.bytes, {}, GIPCE_UNKNOWN);
    SCLF_GET_VALID_ABS_NAMED(enum gnwIpcError * const, replyErrPtr, abs_queryPtr->replyErrPtr, sizeof(enum gnwIpcError), {}, GIPCE_UNKNOWN);
    SCLF_GET_VALID_NULLABLE_ABS_NAMED(const ptr_t, replyPtr, abs_queryPtr->replyData.ptr, abs_queryPtr->replyData.bytes, {}, GIPCE_UNKNOWN);

    struct gnwIpcSenderQuery absoluteQuery;
    absoluteQuery.procId = abs_queryPtr->procId;
    absoluteQuery.pathData.ptr = (ptr_t)abs_pathPtr;
    absoluteQuery.pathData.bytes = abs_queryPtr->pathData.bytes;
    absoluteQuery.data.ptr = abs_dataPtr;
    absoluteQuery.data.bytes = abs_queryPtr->data.bytes;
    absoluteQuery.replyErrPtr = abs_replyErrPtr;
    absoluteQuery.replyData.ptr = abs_replyPtr;
    absoluteQuery.replyData.bytes = abs_queryPtr->replyData.bytes;
    absoluteQuery.bindData = abs_queryPtr->bindData;

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

    return k_ipc_send(procId, absoluteQuery);
}