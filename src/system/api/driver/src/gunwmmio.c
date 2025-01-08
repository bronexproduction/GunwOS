//
//  gunwmmio.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwmmio.h"
#include <defs.h>
#include <gunwfug.h>
#include <scl_driver.h>

ptr_t mmioPlz(const addr_t physMemStart, const size_t sizeBytes, enum gnwDeviceError * const errorPtr) {
    CHECKPTR(errorPtr);
    
    #warning HOW THE MAPPED MEMORY AFFECTS DYNAMIC MEMORY ALLOCATION?
    #warning possible overlap with current heap (see gunwmem.c)
    #warning the heap should be moved or some restrictions to be applied

    SYSCALL_DRIVER_CALL(MMIO_PLZ, physMemStart, sizeBytes, errorPtr);
    
    return (ptr_t)SYSCALL_RESULT;
}

#endif // _GUNWAPI_KERNEL
