//
//  fug.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.02.2023.
//

#include "fug.h"
#include <string.h>
#include <error/panic.h>
#include <hal/proc/proc.h>
#include <gunwoutput.h>

void k_err_fug(const procId_t procId, enum gnwFugCode code) {
    logfn("Process {i} crashed with code {h}", procId, code);
    k_proc_stop(procId);
}
