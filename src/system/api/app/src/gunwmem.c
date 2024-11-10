//
//  gunwmem.c
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2024.
//

#include "../include/gunwmem.h"
#include <defs.h>
#include "../include/gunwfug.h"
#include "scl_user.h"

#ifndef _GUNWAPI_KERNEL

#define PAGE_SIZE KiB(4)

addr_t _heap;

ptr_t memPlz(const size_t sizeBytes) {
    const enum gnwMemoryError error = memPagePlz(aligned(sizeBytes, PAGE_SIZE) / PAGE_SIZE, _heap);

    if (error != GME_NONE) {
        fug(FUG_NULLPTR);
        return nullptr;
    }
    
    const ptr_t result = (ptr_t)_heap;
    _heap += aligned(sizeBytes, PAGE_SIZE);

    return result;
}

void memThx(const ptr_t ptr) {
    #warning TODO
}

enum gnwMemoryError memPagePlz(const size_t pageCount, const addr_t start) {
    CHECKPTR(start);

    SYSCALL_USER_CALL(MEM_PLZ, pageCount, start, 0);

    return SYSCALL_RESULT;
}

enum gnwMemoryError memPageThx(const addr_t start) {
    CHECKPTR(start);

    SYSCALL_USER_CALL(MEM_THX, start, 0, 0);

    return SYSCALL_RESULT;
}

#endif // _GUNWAPI_KERNEL
