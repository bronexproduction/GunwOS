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

void k_err_fug(const procId_t procId, enum gnwFugCode code) {
    char log[30] = "FUG code      Process ID     ";

    uint2hex(code, log + 9);
    uint2hex(procId, log + 25);
    
    k_log_log(log);
    
    extern void k_scr_usr_exit(procId_t, int_32);
    k_scr_usr_exit(procId, code);
}
