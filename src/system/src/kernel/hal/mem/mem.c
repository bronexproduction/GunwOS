//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#include "mem.h"
#include <hal/proc/proc.h>

struct k_mem_zone k_mem_zoneForProc(procId_t procId) {
    enum k_gdt_dpl dpl = k_proc_getInfo(k_proc_getCurrentId()).dpl;
    
    struct k_mem_zone result;
    switch (dpl) {
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
            result.startPtr = 0;
            result.endPtr = 0;
            result.sizeBytes = 0;
    }

    return result;
}
