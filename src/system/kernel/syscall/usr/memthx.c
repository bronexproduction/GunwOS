//
//  memthx.c
//  GunwOS
//
//  Created by Artur Danielewski on 05.11.2024.
//

#include <src/_gunwmem.h>
#include <hal/paging/paging.h>
#include <hal/mem/mem.h>

enum gnwMemoryError k_scr_usr_memThx(const procId_t procId, const addr_t addr) {
    if (addr % MEM_PAGE_SIZE_BYTES) {
        return GME_ADDRESS_ALIGNMENT;
    }

    const enum k_mem_error error = k_mem_thanksPage(procId, addr / MEM_PAGE_SIZE_BYTES);
    
    if (error != ME_NONE) {
        return GME_UNKNOWN;
    }

    return GME_NONE;
}
