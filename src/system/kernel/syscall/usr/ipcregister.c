//
//  ipcregister.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.04.2023.
//

#include <gunwevent.h>
#include <ipc/ipc.h>
#include <hal/proc/proc.h>
#include <src/_gunwipc.h>
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcRegister(const struct gnwIpcHandlerDescriptor * descPtr) {

    if (!descPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!descPtr->pathData.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!descPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    if (!descPtr->handlerRoutine) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!descPtr->decoder) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }

    return k_ipc_register(k_proc_getCurrentId(),
                          descPtr->pathData,
                          descPtr->handlerRoutine,
                          descPtr->decoder,
                          descPtr->bindingRequired,
                          descPtr->permissionMask);
}