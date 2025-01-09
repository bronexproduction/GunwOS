//
//  mmioplz.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.04.2023.
//

#include <defs.h>
#include <gunwdevtypes.h>
#include <error/panic.h>
#include <hal/paging/paging.h>
#include <hal/mem/mem.h>
#include <dev/dev.h>

ptr_t k_scr_drv_mmioPlz(const procId_t procId,
                        const addr_t physMemStart,
                        const size_t sizeBytes,
                        enum gnwDeviceError * const vErrPtr) {
    
    if (!vErrPtr) {
        OOPS("Unexpected null pointer", nullptr);
    }
    if (!k_mem_bufferZoneValidForProc(procId, (ptr_t)vErrPtr, sizeof(enum gnwDeviceError))) {
        OOPS("Reserved zone access violation", nullptr);
    }
    if (!sizeBytes) {   
        MEM_ONTABLE(procId, 
            *(vErrPtr) = GDE_INVALID_PARAMETER;
        )
    }
    
    const size_t pageAlignedPhysMemStart = alignedr(physMemStart, MEM_PAGE_SIZE_BYTES, false);
    const size_t pageAlignmentStartDiff = physMemStart - pageAlignedPhysMemStart;
    const size_t pageAlignedSizeBytes = alignedr(sizeBytes + pageAlignmentStartDiff, MEM_PAGE_SIZE_BYTES, true);
    if (pageAlignedSizeBytes < sizeBytes) {
        MEM_ONTABLE(procId, 
            *(vErrPtr) = GDE_INVALID_PARAMETER;
        )
        return nullptr;
    }

    MEM_ONTABLE(procId, 
        *(vErrPtr) = GDE_NONE;
    )

    if (!k_dev_mmioRangeAllowed(procId, pageAlignedPhysMemStart, pageAlignedSizeBytes)) {
        MEM_ONTABLE(procId, 
            *(vErrPtr) = GDE_INVALID_PARAMETER;
        )
        return nullptr;   
    }
    if (!k_mem_bufferIsInUsableUmaRange(pageAlignedPhysMemStart, pageAlignedSizeBytes)) {
        MEM_ONTABLE(procId, 
            *(vErrPtr) = GDE_INVALID_PARAMETER;
        )
        return nullptr;   
    }

    #warning TODO try to map desired memory region

    if (true /* failure */) {
        MEM_ONTABLE(procId, 
            *(vErrPtr) = GDE_UNKNOWN;
        )
        return nullptr;
    }
}

/*
#include <_gunwmem.h>
#include <hal/paging/paging.h>
#include <hal/mem/mem.h>

enum gnwMemoryError k_scr_usr_memPlz(const procId_t procId, const size_t pageCount, const addr_t addr) {
    if (addr % MEM_PAGE_SIZE_BYTES) {
        return GME_ADDRESS_ALIGNMENT;
    }

    const enum k_mem_error error = k_mem_gimme(procId, (ptr_t)addr, pageCount * MEM_PAGE_SIZE_BYTES);
    if (error == ME_ALREADY_ASSIGNED ||
        error == ME_PART_ALREADY_ASSIGNED ||
        error == ME_NONE) {
        return GME_NONE;
    } else if (error == ME_OUT_OF_PAGES ||
               error == ME_OUT_OF_MEMORY) {
        return GME_OUT_OF_MEMORY;
    }
    
    return GME_UNKNOWN;
}
*/