//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#include <hal/proc/proc.h>
#include <log/log.h>
#include "../func.h"

void k_scr_usr_log(const char * const msgPtr, const size_t msgBytes) {
    
    const procId_t procId = k_proc_getCurrentId();
    SCLF_GET_VALID_ABS(const char * const, msgPtr, msgBytes, {
        OOPS("scr_usr_log: access violation");
        return;
    });

    if (!abs_msgPtr) {
        OOPS("scr_usr_log: access violation");
        return;
    }

    data_t absLogData;
    absLogData.ptr = (ptr_t)abs_msgPtr;
    absLogData.bytes = msgBytes;

    LOG_START;
    k_log_logd(absLogData);
    LOG_END;
}
