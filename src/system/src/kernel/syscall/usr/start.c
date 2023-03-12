//
//  start.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <gunwctrl.h>
#include <types.h>
#include <string.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <gunwelf.h>

enum gnwCtrlError k_scr_usr_start(const char * const path, const size_t pathLen) {
    if (!path) {
        return GCE_INVALID_ARGUMENT;
    }

    procId_t procId = k_proc_getCurrentId();
    ptr_t absPathPtr = k_mem_absForProc(procId, (const ptr_t)path);

    if (!k_mem_bufInZoneForProc(procId, absPathPtr, pathLen)) {
        return GCE_INVALID_ARGUMENT;
    }

#warning what stage should be queued?

    // Load executable file header

    // Verify header
    
    ptr_t data;
    if (pathLen == 3 && !strcmpl("cli", (const char *)absPathPtr, pathLen)) {
        data = (ptr_t)0x50000;
    } else {
        return GCE_NOT_FOUND;
    }

    const struct elfHeader32 * const headerPtr = (struct elfHeader32 *)data;
    if (!elfValidate(headerPtr)) {
        return GCE_HEADER_INVALID;
    }
    
    // Allocate process memory

    // Load executable
    
    // Spawn process

    #warning TO BE IMPLEMENTED
    return GCE_NONE;
}
