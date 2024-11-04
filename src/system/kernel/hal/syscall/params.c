//
//  params.c
//  GunwOS
//
//  Created by Artur Danielewski on 23.06.2024.
//

#include "params.h"

#include <hal/mem/mem.h>
#include <error/panic.h>

void * userStackSafeValuePointer(const procId_t procId, 
                                 const ptr_t refEsp,
                                 const addr_t offset,
                                 const size_t bytes) {
    ptr_t valPtr = _USER_STACK_PTR + offset;
    
    if (!k_mem_bufferZoneValidForProc(procId, valPtr, bytes)) {
        OOPS("Reserved zone stack access violation", nullptr);
    }

    return valPtr;
}
