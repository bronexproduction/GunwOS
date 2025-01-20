//
//  replysetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

void k_scr_drv_replySetParam(const procId_t procId,
                             const bool success) {

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier",);
        k_proc_stop(procId);
        return;
    }

    k_dev_setParam_reply(procId, desc.identifier, success);
}
