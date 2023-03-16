//
//  func.c
//  GunwOS
//
//  Created by Artur Danielewski on 16.03.2023.
//

#include "func.h"
#include <hal/proc/proc.h>
#include <hal/mem/mem.h>
#include <error/panic.h>

ptr_t k_scl_func_getValidAbsoluteForProc(const procId_t procId, const ptr_t relPtr, const size_t refSize) {
    if (!relPtr) {
        OOPS("Null pointer referenced");
    }

    const ptr_t absPtr = k_mem_absForProc(procId, (const ptr_t)relPtr);
    
    if (!k_mem_bufInZoneForProc(procId, absPtr, refSize)) {
        OOPS("Access violation");
    }

    return absPtr;
}

ptr_t k_scl_func_getValidAbsoluteForCurrentProc(const ptr_t relPtr, const size_t refSize) {
    return k_scl_func_getValidAbsoluteForProc(k_proc_getCurrentId(), relPtr, refSize);
}
