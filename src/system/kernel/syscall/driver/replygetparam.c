//
//  replygetparam.c
//  GunwOS
//
//  Created by Artur Danielewski on 14.01.2025.
//

#include <dev/dev.h>
#include <error/panic.h>

void k_scr_drv_replyGetParam(const procId_t procId,
                             const bool success,
                             const size_t result) {

    struct gnwDeviceUHADesc desc;
    const enum gnwDeviceError error = k_dev_getByOperator(procId, &desc);
    if (error != GDE_NONE) {
        OOPS("Unexpected device operator identifier",);
        #warning TODO k_scr_usr_bye should not be called directly
        extern void k_scr_usr_bye(const procId_t, const int_32);
        k_scr_usr_bye(procId, -69);
        return;
    }

    k_dev_getParam_reply(procId, desc.identifier, success, result);
}
