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

extern addr_t _heapStart;

ptr_t mmioPlz(const addr_t physMemStart, const size_t sizeBytes, enum gnwMemoryError * const errorPtr) {
    CHECKPTR(errorPtr);

    if (isHeapUsed()) {
        *(errorPtr) = GME_HEAP_ALREADY_USED;
        return nullptr;
    }

    const addr_t vAddr = aligned(_heapStart, PAGE_SIZE);
    const addr_t pAddr = alignedr(physMemStart, PAGE_SIZE, false);
    const size_t pageCount = aligned(physMemStart + sizeBytes - pAddr, PAGE_SIZE) / PAGE_SIZE;

    SYSCALL_DRIVER_CALL(MMIO_PLZ, pageCount, vAddr, pAddr);

    *(errorPtr) = SYSCALL_RESULT;
    if (*(errorPtr) != GME_NONE) {
        return nullptr;
    }

    _heapStart = (vAddr + (pageCount * PAGE_SIZE));
    return (ptr_t)(vAddr + (physMemStart - pAddr));
}

#endif // _GUNWAPI_KERNEL
