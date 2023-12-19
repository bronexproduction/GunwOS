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
    SCLF_GET_VALID_ABS_NAMED(const char * const, pathPtr, abs_queryPtr->path, abs_queryPtr->pathLen, {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(const ptr_t, dataPtr, abs_queryPtr->dataPtr, abs_queryPtr->dataSizeBytes, {
        return GIPCE_UNKNOWN;
    });
    SCLF_GET_VALID_ABS_NAMED(enum gnwIpcError * const, replyErrPtr, abs_queryPtr->replyErrPtr, sizeof(enum gnwIpcError), {
        return GIPCE_UNKNOWN;
    });

    ptr_t abs_replyPtr = nullptr;
    if (abs_queryPtr->replyPtr) {
        abs_replyPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(abs_queryPtr->replyPtr), abs_queryPtr->replySizeBytes);
        if (!abs_replyPtr) {
            OOPS("Invalid pointer referenced");
            return GIPCE_UNKNOWN;
        }
    }

    struct gnwIpcSenderQuery absoluteQuery;
    absoluteQuery.procId = abs_queryPtr->procId;
    absoluteQuery.path = abs_pathPtr;
    absoluteQuery.pathLen = abs_queryPtr->pathLen;
    absoluteQuery.dataPtr = abs_dataPtr;
    absoluteQuery.dataSizeBytes = abs_queryPtr->dataSizeBytes;
    absoluteQuery.replyErrPtr = abs_replyErrPtr;
    absoluteQuery.replyPtr = abs_replyPtr;
    absoluteQuery.replySizeBytes = abs_queryPtr->replySizeBytes;
    absoluteQuery.bindFlag = abs_queryPtr->bindFlag;
    absoluteQuery.permissions = abs_queryPtr->permissions;

    return k_ipc_send(procId, absoluteQuery);
}