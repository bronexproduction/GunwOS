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

static struct heapMetadataEntry {
    addr_t addr;
    size_t size;
    struct heapMetadataEntry * next;
} *heapMetadataStartEntry;

addr_t _heapStart;

static ptr_t allocate(struct heapMetadataEntry * prev, 
                      struct heapMetadataEntry * next,
                      const size_t sizeBytes) {


    // #warning TODO
    
    // const enum gnwMemoryError error = memPagePlz(aligned(sizeBytes, PAGE_SIZE) / PAGE_SIZE, _heap);

    // if (error != GME_NONE) {
    //     fug(FUG_NULLPTR);
    //     return nullptr;
    // }
    
    // const ptr_t result = (ptr_t)_heap;
    // _heap += aligned(sizeBytes, PAGE_SIZE);

    // return result;
}

ptr_t memPlz(const size_t sizeBytes) {

    if (!sizeBytes) {
        fug(FUG_UNDEFINED);
        return nullptr;
    }

    size_t allocSize = sizeBytes + sizeof(struct heapMetadataEntry);

    if (_heapStart + allocSize <= _heapStart) {
        fug(FUG_OUT_OF_MEMORY);
        return nullptr;
    }
    
    if (!heapMetadataStartEntry) {
        /*
            Alloc as heap start
        */
        return allocate(nullptr, nullptr, allocSize);
    } else if (_heapStart + allocSize <= heapMetadataStartEntry->addr) {
        /*
            Alloc at the beginning
        */
        return allocate(nullptr, heapMetadataStartEntry, allocSize);
    } else {
        /*
            Find item to alloc after
        */
        struct heapMetadataEntry * entry = heapMetadataStartEntry;

        while (entry->next) {
            addr_t gapStart = entry->addr + entry->size;
            addr_t nextUnit = gapStart + allocSize;

            if (nextUnit <= gapStart) {
                fug(FUG_OUT_OF_MEMORY);
                return nullptr;
            }
            if (nextUnit <= entry->next->addr) {
                break;
            }

            entry = entry->next;
        }

        return allocate(entry, entry->next, allocSize);
    }
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
