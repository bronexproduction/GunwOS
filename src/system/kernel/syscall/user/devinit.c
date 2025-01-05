//
//  devinit.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.01.2024.
//

// #include <_gunwdev.h>
// #include <_gunwctrl.h>
// #include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <gunwdevtypes.h>
#include <gunwdrvtypes.h>
// #include <error/panic.h>
// #include <prog/prog.h>
// #include <queue/queue.h>
#include <log/log.h>

void k_scr_usr_devInit(const procId_t procId,
                       const procId_t operatorProcId,
                       enum gnwDriverError * vErrorPtr) {
                        LOG("k_scr_usr_devInit");
    // if (!descPtr) {
    //     OOPS("Unexpected nullptr",);
    // }
    // if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr, sizeof(struct gnwDeviceInstallDescriptor))) {
    //     OOPS("Reserved zone access violation",);
    // }
    // if (!descPtr->errorPtr) {
    //     OOPS("Unexpected nullptr",);
    // }
    // if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->errorPtr, sizeof(enum gnwDeviceInstallError))) {
    //     OOPS("Reserved zone access violation",);
    // }
    // if (!descPtr->ctrlDesc.errorPtr) {
    //     OOPS("Unexpected nullptr",);
    // }
    // if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.errorPtr, sizeof(enum gnwCtrlError))) {
    //     OOPS("Reserved zone access violation",);
    // }

    // *(descPtr->errorPtr) = GDIE_NONE;

    // if (!descPtr->ctrlDesc.pathData.ptr) {
    //     *(descPtr->ctrlDesc.errorPtr) = GCE_INVALID_ARGUMENT;
    //     OOPS("Unexpected nullptr",);
    // }
    // if (!descPtr->ctrlDesc.pathData.bytes) {
    //     *(descPtr->ctrlDesc.errorPtr) = GCE_INVALID_ARGUMENT;
    //     OOPS("Unexpected length",);
    // }
    // if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)descPtr->ctrlDesc.pathData.ptr, descPtr->ctrlDesc.pathData.bytes)) {
    //     OOPS("Reserved zone access violation",);
    // }

    // k_que_dispatch_arch4((fPtr_arch4)(void *)k_prog_spawnDriver,
    //                      procId,
    //                      (addr_t)(&descPtr->ctrlDesc.pathData),
    //                      (addr_t)(descPtr->errorPtr),
    //                      (addr_t)(descPtr->ctrlDesc.errorPtr));
    // k_proc_lock(procId, PLT_SYNC);
}



/*



     
    //    Perform driver installation and startup
    

    enum gnwDriverError e;
    size_t deviceId;
    e = k_dev_install_async(deviceDescriptorPtr, spawnedProcId, &deviceId);
    if (e != GDRE_NONE) { 
        k_proc_stop(spawnedProcId);
        MEM_ONTABLE(procId,
            *(vInstallErrorPtr) = GDIE_INSTALLATION_FAILED;
        )
        OOPS("Driver installation failed",);
    }


*/


