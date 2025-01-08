//
//  mmioplz.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.04.2023.
//

#warning to be removed
#include <gunwdevtypes.h>

ptr_t k_scr_drv_mmioPlz(const procId_t procId,
                        const addr_t physMemStart,
                        const size_t sizeBytes,
                        enum gnwDeviceError * const vErrPtr) {

    #warning todo
    return GDE_NONE;
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