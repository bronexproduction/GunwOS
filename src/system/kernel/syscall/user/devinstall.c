//
//  devinstall.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#include <_gunwdev.h>
#include <_gunwctrl.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>
#include <prog/prog.h>

void k_scr_usr_devInstall(const procId_t procId,
                          const struct gnwDeviceInstallDescriptor * const descPtr) {
    if (!descPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwDeviceInstallDescriptor))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->errorPtr, sizeof(enum gnwDeviceInstallError))) {
        OOPS("Reserved zone access violation",);
    }
    if (!descPtr->ctrlDesc.errorPtr) {
        OOPS("Unexpected nullptr",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.errorPtr, sizeof(enum gnwCtrlError))) {
        OOPS("Reserved zone access violation",);
    }

    *(descPtr->errorPtr) = GDIE_NONE;

    if (!descPtr->ctrlDesc.pathData.ptr) {
        *(descPtr->ctrlDesc.errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected nullptr",);
    }
    if (!descPtr->ctrlDesc.pathData.bytes) {
        *(descPtr->ctrlDesc.errorPtr) = GCE_INVALID_ARGUMENT;
        OOPS("Unexpected length",);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.pathData.ptr, descPtr->ctrlDesc.pathData.bytes)) {
        OOPS("Reserved zone access violation",);
    }

    enum gnwCtrlError ctrlErr = k_prog_spawnDriver(descPtr->ctrlDesc.pathData, descPtr->errorPtr);
    if (ctrlErr != GCE_NONE) {
        *(descPtr->ctrlDesc.errorPtr) = ctrlErr;
        return;
    }
    
    #warning how to store description pointer for further use?

    k_proc_lock(procId, PLT_SYNC);
}
