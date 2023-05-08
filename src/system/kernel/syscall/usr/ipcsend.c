//
//  ipcsend.c
//  GunwOS
//
//  Created by Artur Danielewski on 15.04.2023.
//

#include <syscall/func.h>
#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <defs.h>

enum gnwIpcError k_scr_usr_ipcSend(const struct gnwIpcSenderQuery * const queryPtr) {

    const procId_t procId = k_proc_getCurrentId();
    const struct gnwIpcSenderQuery * const absQueryPtr = (struct gnwIpcSenderQuery *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)queryPtr, sizeof(struct gnwIpcSenderQuery));
    if (!absQueryPtr) {
        OOPS("Invalid pointer referenced");
        return GIPCE_UNKNOWN;
    }
    const char * const absPathPtr = (char *)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(absQueryPtr->path), absQueryPtr->pathLen);
    if (!absPathPtr) {
        OOPS("Invalid pointer referenced");
        return GIPCE_UNKNOWN;
    }
    const ptr_t absDataPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(absQueryPtr->params.dataPtr), absQueryPtr->params.dataBytes);
    if (!absDataPtr) {
        OOPS("Invalid pointer referenced");
        return GIPCE_UNKNOWN;
    }
    ptr_t absResultPtr = nullptr;
    if (absQueryPtr->params.resultPtr) {
        absResultPtr = k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(absQueryPtr->params.resultPtr), absQueryPtr->params.resultBytes);
        if (!absResultPtr) {
            OOPS("Invalid pointer referenced");
            return GIPCE_UNKNOWN;
        }
    }

    struct gnwIpcSenderQuery absoluteQuery;
    absoluteQuery.path = absPathPtr;
    absoluteQuery.pathLen = absQueryPtr->pathLen;
    absoluteQuery.params.dataPtr = absDataPtr;
    absoluteQuery.params.dataBytes = absQueryPtr->params.dataBytes;
    absoluteQuery.params.resultPtr = absResultPtr;
    absoluteQuery.params.resultBytes = absQueryPtr->params.resultBytes;

    return k_ipc_ipcSend(procId, absoluteQuery);
}