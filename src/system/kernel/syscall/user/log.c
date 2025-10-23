//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.12.2023.
//

#include <hal/mem/mem.h>
#include <error/panic.h>
#include <gunwoutput.h>

void k_scr_usr_log(const procId_t procId, const char * const msgPtr, const size_t msgBytes) {

    MEM_VALIDATE_VPTR_BUFFER(procId, msgPtr, msgBytes,,);
    
    logln(msgPtr, msgBytes);
}
