//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#include <log/log.h>
#include <error/panic.h>

void k_scr_usr_log(const char * const msgPtr, const size_t msgBytes) {
    
    if (!msgPtr) {
        OOPS("Unexpected null pointer",);
    }
    if (!msgBytes) {
        OOPS("Unexpected message size",);
    }

    data_t logData;
    logData.ptr = (ptr_t)msgPtr;
    logData.bytes = msgBytes;

    LOG_START;
    k_log_logd(logData);
    LOG_END;
}
