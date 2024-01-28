//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#include "mem.h"
#include <mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

struct k_mem_zone {
    ptr_t startPtr;
    ptr_t endPtr;
    size_t sizeBytes;
};

static struct k_mem_zone zoneForProc(const procId_t procId) {
    struct k_proc_process proc = k_proc_getInfo(procId);
    struct k_mem_zone result;
    memzero(&result, sizeof(struct k_mem_zone));

    if (proc.state == PS_NONE) {
        OOPS("Invalid process state", result);
    }
    
    switch (proc.dpl) { 
        case DPL_0:
            result.startPtr = GDT_SEGMENT_START(r0data);
            result.endPtr = GDT_SEGMENT_END(r0data);
            result.sizeBytes = GDT_LIMIT_BYTES(r0data);
            break;
        case DPL_3:
            result.startPtr = GDT_SEGMENT_START(r3data);
            result.endPtr = GDT_SEGMENT_END(r3data);
            result.sizeBytes = GDT_LIMIT_BYTES(r3data);
            break;
        default:
            break;
    }

    return result;
}

ptr_t k_mem_absForProc(const procId_t procId, const ptr_t relPtr) {
    if (!relPtr) {
        OOPS("Invalid pointer", nullptr);
    }

    return relPtr + (addr_t)zoneForProc(procId).startPtr;
}

bool k_mem_bufInZoneForProc(const procId_t procId, const ptr_t absPtr, const size_t bufSize) {
    // check buffer bounds
    // there might be a better way to do it
    // but will do, at least until virtual memory gets implemented
    const struct k_mem_zone procZone = zoneForProc(procId);
    if (absPtr < procZone.startPtr || procZone.endPtr < absPtr) {
        return false;
    }
    
    const size_t trailingBytes = procZone.endPtr - absPtr + 1;
    if (bufSize > trailingBytes) {
        return false;
    }

    return true;
}

void k_mem_procCleanup(const procId_t procId) {
    #warning NOTHING TO BE DONE YET
}
