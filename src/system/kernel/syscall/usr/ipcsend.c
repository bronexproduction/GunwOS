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

enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr) {

    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const struct gnwIpcSenderQuery * const, queryPtr, sizeof(struct gnwIpcSenderQuery), {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(const gnwIpcPath, pathPtr, abs_queryPtr->pathData.ptr, abs_queryPtr->pathData.bytes, {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_NULLABLE_ABS_NAMED(const ptr_t, dataPtr, abs_queryPtr->data.ptr, abs_queryPtr->data.bytes, {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(enum gnwIpcError * const, replyErrPtr, abs_queryPtr->replyErrPtr, sizeof(enum gnwIpcError), {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_NULLABLE_ABS_NAMED(const ptr_t, replyPtr, abs_queryPtr->replyData.ptr, abs_queryPtr->replyData.bytes, {
        return GIPCE_UNKNOWN;
    });

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

    return k_ipc_send(procId, absoluteQuery);
}