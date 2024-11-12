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
    size_t dataSizeBytes;
    struct heapMetadataEntry * next;
} *heapMetadataStartEntry;

addr_t _heapStart;

static ptr_t allocate(struct heapMetadataEntry * prev, 
                      struct heapMetadataEntry * next,
                      const size_t dataSizeBytes) {

    struct heapMetadataEntry * newEntry = (struct heapMetadataEntry *)(
        prev ? ((addr_t)prev + sizeof(struct heapMetadataEntry) + prev->dataSizeBytes) : _heapStart
    );

    /*
        Ask system for pages
    */

    addr_t startPageAddr = alignedr((size_t)newEntry, PAGE_SIZE, false);
    addr_t followingPageAddress = alignedr((size_t)newEntry + sizeof(struct heapMetadataEntry) + dataSizeBytes, PAGE_SIZE, true);
    size_t pageCount = (followingPageAddress - startPageAddr) / PAGE_SIZE;

    const enum gnwMemoryError error = memPagePlz(pageCount, startPageAddr);
    if (error == GME_OUT_OF_MEMORY) {
        fug(FUG_OUT_OF_MEMORY);
        return nullptr;
    } else if (error != GME_NONE) {
        fug(FUG_UNDEFINED);
        return nullptr;
    }

    /*
        Update list
    */

    newEntry->next = next;
    newEntry->dataSizeBytes = dataSizeBytes;
    
    if (prev) {
        prev->next = newEntry;
    }

    return (ptr_t)(newEntry + sizeof(struct heapMetadataEntry));
}

ptr_t memPlz(const size_t sizeBytes) {

    if (!sizeBytes) {
        fug(FUG_UNDEFINED);
        return nullptr;
    }

    size_t allocSize = sizeBytes + sizeof(struct heapMetadataEntry);
    if (allocSize < sizeBytes || !allocSize) {
        fug(FUG_UNDEFINED);
        return nullptr;
    }

    if (_heapStart + allocSize <= _heapStart) {
        fug(FUG_OUT_OF_MEMORY);
        return nullptr;
    }
    
    if (!heapMetadataStartEntry) {
        /*
            Alloc as heap start
        */
        return allocate(nullptr, nullptr, sizeBytes);
    } else if (_heapStart + allocSize <= (addr_t)heapMetadataStartEntry) {
        /*
            Alloc at the beginning
        */
        return allocate(nullptr, heapMetadataStartEntry, sizeBytes);
    } else {
        /*
            Find item to alloc after
        */
        struct heapMetadataEntry * entry = heapMetadataStartEntry;

        while (entry->next) {
            addr_t gapStart = (addr_t)entry + sizeof(struct heapMetadataEntry) + entry->dataSizeBytes;
            addr_t nextUnit = gapStart + allocSize;

            if (nextUnit <= gapStart) {
                fug(FUG_OUT_OF_MEMORY);
                return nullptr;
            }
            if (nextUnit <= (addr_t)entry->next) {
                break;
            }

            entry = entry->next;
        }

        return allocate(entry, entry->next, sizeBytes);
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
