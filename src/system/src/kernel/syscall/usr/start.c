//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <gunwctrl.h>
#include <types.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>

enum gnwCtrlError k_scr_usr_start(const char * const path, const size_t pathLen) {
    if (!path) {
        return GCE_INVALID_ARGUMENT;
    }

    procId_t procId = k_proc_getCurrentId();
    ptr_t absPathPtr = k_mem_absForProc(procId, (const ptr_t)path);

    if (!k_mem_bufInZoneForProc(procId, absPathPtr, pathLen)) {
        return GCE_INVALID_ARGUMENT;
    }

    #warning TO BE IMPLEMENTED
    return GCE_NONE;
}
