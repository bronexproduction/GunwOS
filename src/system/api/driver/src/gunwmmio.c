//
//  gunwmmio.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwmmio.h"
#include <_gunwmem.h>
#include <defs.h>
#include <gunwfug.h>
#include <scl_driver.h>

extern const addr_t _heapStart;

ptr_t mmioPlz(const addr_t physMemStart, const size_t sizeBytes, enum gnwMemoryError * const errorPtr) {
    CHECKPTR(errorPtr);

    if (isHeapUsed()) {
        *(errorPtr) = GME_HEAP_ALREADY_USED;
        return nullptr;
    }

    const addr_t vAddr = aligned(_heapStart, PAGE_SIZE);
    const addr_t pAddr = physMemStart / PAGE_SIZE;
    const size_t pageCount = aligned(physMemStart + sizeBytes - pAddr, PAGE_SIZE) / PAGE_SIZE;

    SYSCALL_DRIVER_CALL(MMIO_PLZ, pageCount, vAddr, pAddr);

    *(errorPtr) = SYSCALL_RESULT;
    if (*(errorPtr) != GME_NONE) {
        return nullptr;
    }

    #warning SUCCESS! DETS OSOM DUMOR
    #warning TODO UPDATE _HEAPSTART
    
    return nullptr;
}

#endif // _GUNWAPI_KERNEL
