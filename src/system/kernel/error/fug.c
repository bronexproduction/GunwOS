//
//  fug.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.02.2023.
//

#include "fug.h"
#include <log/log.h>
#include <string.h>
#include <error/panic.h>
#include <hal/proc/proc.h>

void k_err_fug(const procId_t procId, enum gnwFugCode code) {
    char log[37] = "Process       crashed with code     ";
    
    hex2str(procId, log + 8);
    hex2str(code, log + 32);
    
    LOG(log);
    
    k_proc_stop(procId);
}
