//
//  utility.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.12.2023.
//

#include "utility.h"
#include "data.h"
#include <mem.h>
#include <string.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

bool k_ipc_utl_pathGlobalValidate(const data_t absPathData) {
    /*
        Path empty
    */
    if (!absPathData.bytes) {
        return false;
    }
    
    /*
        Path starting with separator
    */
    if (absPathData.ptr[0] == GNW_PATH_IPC_COMPONENT_SEPARATOR) {
        return false;
    }
    
    /*
        Path ending with separator
    */
    if (absPathData.ptr[absPathData.bytes - 1] == GNW_PATH_IPC_COMPONENT_SEPARATOR) {
        return false;
    }

    /*
        Path containing invalid characters
        Adjacent separators
    */
    bool hasSeparator = false;
    size_t lastSeparatorIndex = 0;
    for (size_t i = 0; i < absPathData.bytes; ++i) {
        if (!(IN_RANGE('0', absPathData.ptr[i], '9')                          ||   /* 0 - 9 */
              IN_RANGE('A', absPathData.ptr[i], 'Z')                          ||   /* A - Z */
              IN_RANGE('a', absPathData.ptr[i], 'z')                          ||   /* a - z */
              absPathData.ptr[i] == GNW_PATH_IPC_COMPONENT_SEPARATOR               /* path separator */)) {
            return false;
        }
        if (absPathData.ptr[i] != GNW_PATH_IPC_COMPONENT_SEPARATOR) {
            continue;
        }
        if (lastSeparatorIndex == (i - 1) && hasSeparator) {
            return false;
        }
        hasSeparator = true;
        lastSeparatorIndex = i;
    }

    return true;
}

bool k_ipc_utl_pathDirectValidate(data_t absPathData) {
    if (absPathData.bytes <= 2) {
        return false;
    }
    if (absPathData.ptr[0] != GNW_PATH_IPC_DIRECT_PREFIX) {
        return false;
    }
    if (absPathData.ptr[1] != GNW_PATH_IPC_COMPONENT_SEPARATOR) {
        return false;
    }

    absPathData.ptr += 2;
    absPathData.bytes -= 2;
    return k_ipc_utl_pathGlobalValidate(absPathData);
}

bool k_ipc_utl_pathNotificationValidate(data_t absPathData) {
    if (absPathData.bytes <= 2) {
        return false;
    }
    if (absPathData.ptr[0] != GNW_PATH_IPC_NOTIFICATION_PREFIX) {
        return false;
    }
    if (absPathData.ptr[1] != GNW_PATH_IPC_COMPONENT_SEPARATOR) {
        return false;
    }

    absPathData.ptr += 2;
    absPathData.bytes -= 2;
    return k_ipc_utl_pathGlobalValidate(absPathData);
}

void k_ipc_utl_clearReply(const size_t entryId) {
    if (entryId >= MAX_IPC_TOKEN) {
        OOPS("Reply index out of bounds",);
    }
    
    memzero(&ipcReplyRegister[entryId], sizeof(struct ipcReply));
    ipcReplyRegister[entryId].senderProcId = NONE_PROC_ID;
    ipcReplyRegister[entryId].handlerProcId = NONE_PROC_ID;
}

void k_ipc_utl_unlockIfAble(const procId_t procId) {
    for (size_t token = 0; token < MAX_IPC_TOKEN; ++token) {
        if (ipcReplyRegister[token].senderProcId == procId) {
            return;
        }
    }

    k_proc_unlock(procId, PLT_IPC);
}

size_t k_ipc_utl_nextListenerIndexForPath(const data_t absPathData, const size_t * const after) {
    for (size_t index = after ? *after + 1 : 0; index < MAX_IPC_LISTENER; ++index) {
        if (!strcmpl((const gnwIpcPath)absPathData.ptr, ipcListenerRegister[index].path, absPathData.bytes)) {
            return index;
        }
    }

    return MAX_IPC_LISTENER;
}
