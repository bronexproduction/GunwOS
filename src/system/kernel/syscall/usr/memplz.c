//
//  memplz.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.11.2024.
//

#include <src/_gunwmem.h>
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
