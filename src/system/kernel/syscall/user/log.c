//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#include <log/log.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

void k_scr_usr_log(const procId_t procId, const char * const msgPtr, const size_t msgBytes) {

    MEM_VALIDATE_VPTR_BUFFER(procId, msgPtr, msgBytes,,);

    data_t logData;
    logData.ptr = (ptr_t)msgPtr;
    logData.bytes = msgBytes;

    LOG_START;
    k_log_logd(logData);
    LOG_END;
}
