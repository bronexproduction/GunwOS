//
//  ipcregister.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.04.2023.
//

#include <gunwevent.h>
#include <ipc/ipc.h>
#include <hal/mem/mem.h>
#include <_gunwipc.h>
#include <error/panic.h>

enum gnwIpcError k_scr_usr_ipcRegister(const procId_t procId, const struct gnwIpcHandlerDescriptor * descPtr) {

    if (!descPtr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwIpcHandlerDescriptor))) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!descPtr->pathData.ptr) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!descPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->pathData.ptr, descPtr->pathData.bytes)) {
        OOPS("Reserved zone access violation", GIPCE_UNKNOWN);
    }
    if (!descPtr->handlerRoutine) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }
    if (!descPtr->decoder) {
        OOPS("Unexpected null pointer", GIPCE_UNKNOWN);
    }

    return k_ipc_register(procId,
                          descPtr->pathData,
                          descPtr->handlerRoutine,
                          descPtr->decoder,
                          descPtr->bindingRequired,
                          descPtr->permissionMask);
}