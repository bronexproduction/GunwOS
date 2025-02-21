//
//  emit.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

enum gnwDeviceError k_scr_drv_emit(const procId_t procId,
                                   const size_t deviceId,
                                   const struct gnwDeviceEvent * const vEventPtr) {

    if (k_dev_operatorOf(deviceId) != procId) {
        k_proc_stop(procId);
        OOPS("Invalid device operator identifier", GDE_ID_INVALID);
    }

    #warning TODO - checks, move to "dev"

    return k_dev_emit(procId, deviceId, vEventPtr);
}
