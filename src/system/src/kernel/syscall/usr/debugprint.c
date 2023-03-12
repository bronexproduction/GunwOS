//
//  debugprint.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <types.h>
#include <string.h>
#include <error/panic.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>

size_t k_scr_usr_debugPrint(uint_32 buffer) {
    // check buffer bounds
    // there might be (must be) a better way to do it
    struct k_mem_zone procZone = k_mem_zoneForProc(k_proc_getCurrentId());
    if (buffer >= procZone.sizeBytes) {
        OOPS("Access violation");
    }
    buffer += (uint_32)procZone.startPtr;
    size_t bufLen = strlen((const char * const)buffer);
    if ((buffer + bufLen) > (size_t)procZone.endPtr) {
        OOPS("Access violation");   
    }

    #warning TO BE IMPLEMENTED - MISSING CHARACTER OUTPUT
    return bufLen;
}
