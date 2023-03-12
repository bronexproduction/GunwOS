//
//  debugprint.c
//  GunwOS
//
//  Created by Artur Danielewski on 12.03.2023.
//

#include <types.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

size_t k_scr_usr_debugPrint(const char * const buffer, const size_t bufferLen) {
    if (!buffer) {
        OOPS("Null pointer refenced");
    }

    const procId_t procId = k_proc_getCurrentId();
    const ptr_t absBufferPtr = k_mem_absForProc(procId, (const ptr_t)buffer);

    if (!k_mem_bufInZoneForProc(procId, absBufferPtr, bufferLen)) {
        OOPS("Access violation");
    }
    
    #warning TO BE IMPLEMENTED - MISSING CHARACTER OUTPUT
    return bufferLen;
}
