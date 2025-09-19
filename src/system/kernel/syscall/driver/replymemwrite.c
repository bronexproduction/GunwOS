//
//  replysetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

void k_scr_drv_replyMemWrite(const procId_t procId,
                             const size_t deviceId,
                             const bool success) {

    if (k_dev_operatorOf(deviceId) != procId) {
        OOPS("Invalid device operator identifier",);
        k_proc_stop(procId);
        return;
    }

    k_dev_writeMem_reply(procId, deviceId, success);
}
