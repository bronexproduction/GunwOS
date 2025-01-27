//
//  emit.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

enum gnwDeviceError k_scr_drv_emit(const procId_t procId, const struct gnwDeviceEvent * const vEventPtr) {

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier", GDE_UNKNOWN);
        k_proc_stop(procId);
        return GDE_UNKNOWN;
    }

    #warning TODO - checks, move to "dev"

    return k_dev_emit(procId, vEventPtr);
}
