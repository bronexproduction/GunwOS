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

    MEM_VALIDATE_VPTR(procId, descPtr, struct gnwIpcHandlerDescriptor,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
    if (!descPtr->pathData.bytes) {
        OOPS("Unexpected path length", GIPCE_UNKNOWN);
    }
    MEM_VALIDATE_VPTR_BUFFER(procId, descPtr->pathData.ptr, descPtr->pathData.bytes,
        { OOPS("Unexpected null pointer", GIPCE_UNKNOWN); },
        { OOPS("Reserved zone access violation", GIPCE_UNKNOWN); }
    )
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