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
    SCLF_GET_VALID_ABS_NAMED(const ptr_t, dataPtr, abs_queryPtr->params.dataPtr, abs_queryPtr->params.dataSizeBytes, {
        return GIPCE_UNKNOWN;
    });

    ptr_t abs_resultPtr = nullptr;
    if (abs_queryPtr->params.resultPtr) {
        abs_resultPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(abs_queryPtr->params.resultPtr), abs_queryPtr->params.resultSizeBytes);
        if (!abs_resultPtr) {
            OOPS("Invalid pointer referenced");
            return GIPCE_UNKNOWN;
        }
    }

    struct gnwIpcSenderQuery absoluteQuery;
    absoluteQuery.path = abs_pathPtr;
    absoluteQuery.pathLen = abs_queryPtr->pathLen;
    absoluteQuery.params.dataPtr = abs_dataPtr;
    absoluteQuery.params.dataSizeBytes = abs_queryPtr->params.dataSizeBytes;
    absoluteQuery.params.resultPtr = abs_resultPtr;
    absoluteQuery.params.resultSizeBytes = abs_queryPtr->params.resultSizeBytes;

    return k_ipc_ipcSend(procId, absoluteQuery);
}