//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <_gunwctrl.h>
#include <hal/mem/mem.h>
#include <error/panic.h>
#include <prog/prog.h>

void k_scr_usr_start(const procId_t procId, struct gnwCtrlStartDescriptor * const descPtr) {
    if (!descPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwCtrlStartDescriptor))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->errorPtr, sizeof(enum gnwCtrlError))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->pathData.ptr) {
        *(descPtr->errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->pathData.bytes) {
        *(descPtr->errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected length",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->pathData.ptr, descPtr->pathData.bytes)) {
        OOPS("Reserved zone access violation",);
    }

    *(descPtr->errorPtr) = k_prog_spawnProgram(descPtr->pathData);
}
