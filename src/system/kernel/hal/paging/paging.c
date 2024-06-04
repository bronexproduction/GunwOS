//
//  paging.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2024.
//

#include "paging.h"
#include "data.h"

#include <types.h>
#include <utils.h>
#include <mem.h>
#include <hal/cpu/cpu.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

#include <string.h>
#include <log/log.h>

static physical_page_table_t physicalPages;
static process_page_tables_t processPageTables;
static __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) virtual_page_table_t kernelPageTables[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];

static virtual_page_table_t * getFreeTable(const procId_t procId) {
    if (!k_proc_idIsUser(procId)) {
        return nullptr;
    }

    for (size_t page = 0; page < MEM_VIRTUAL_USER_PAGE_TABLE_COUNT; ++page) {
        if (!processPageTables[procId].pageTableInfo[page].used) {
            return &processPageTables[procId].pageTables[page];
        }
    }

    return nullptr;
}

static struct virtual_page_specifier_t * getPageSpecifier(const procId_t procId, const size_t page) {
#warning TODO
    return nullptr;
}

static enum k_mem_error getFreePhysicalPage(size_t * const pageIndexPtr) {
#warning TODO
    return ME_OUT_OF_MEMORY;
}

static void assignVirtualPage(struct virtual_page_specifier_t * const pageEntryPtr,
                              const size_t physicalPageIndex,
                              const bool user) {
#warning TODO
}

static void assignDirEntry(struct virtual_page_specifier_t * const dirEntryPtr,
                    const virtual_page_table_t * const pageTablePtr,
                    const bool user) {
    
    addr_t frameAddr = ((addr_t)pageTablePtr) >> 12;
    if ((frameAddr << 12) != (addr_t)pageTablePtr) {
        OOPS("Unexpected page table address",);
    }

    dirEntryPtr->frameAddress = frameAddr;
    dirEntryPtr->present = true;
    dirEntryPtr->writable = true;
    dirEntryPtr->user = user;
}

static void initializePhysicalMemoryMap(const struct k_krn_memMapEntry *memMap) {
    
    /*
        Prepare usable memory map
    */

    for (size_t emsIndex = 0; emsIndex < MMAP_MAX_ENTRIES; ++emsIndex) {
        const struct k_krn_memMapEntry * entry = &memMap[emsIndex];
        if (entry->type == MMRT_NONE) {
            break;
        } else if (entry->type != MMRT_MEMORY &&
                   entry->type != MMRT_RESERVED) {
            OOPS("Mem map inconsistency",);
        }

        bool limit = false;

        if (entry->baseAddrHigh) {
            /*
                Entry over 4GiB boundary - to be ignored
            */
            continue;
        } else if (entry->lengthHigh || (entry->baseAddrLow + entry->lengthLow) < entry->baseAddrLow) {
            /*
                Entry reaches 4GiB boundary - to be limited
            */
            limit = true;
        }

        const bool usable = entry->type == MMRT_MEMORY;
        const addr_t pageAlignedStart = alignedr(entry->baseAddrLow, MEM_PAGE_SIZE_BYTES, usable);
        if (pageAlignedStart < entry->baseAddrLow && usable) {
            /*
                Wraparound
            */
            continue;
        }

        const addr_t pageAlignedStartNext = limit ? 0 : alignedr(entry->baseAddrLow + entry->lengthLow, MEM_PAGE_SIZE_BYTES, !usable);
        const bool flat = !pageAlignedStartNext && !pageAlignedStart;

        if (flat) {
            #warning TODO
            // All memory mapped
            // or only a tiny area at the bottom of memory
            //
            // First case: allow
            // Second case: continue;
        } else if (pageAlignedStartNext <= pageAlignedStart && !limit) {
            continue;
        }

        const addr_t lengthPages = flat ? MEM_MAX_PHYSICAL_PAGE_COUNT : ((pageAlignedStartNext - pageAlignedStart) / MEM_PAGE_SIZE_BYTES);
        const addr_t startPage = flat ? 0 : (pageAlignedStart / MEM_PAGE_SIZE_BYTES);
        
        for (size_t page = startPage; page < (startPage + lengthPages) && page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
            /*
                Kernel reserved area fixed at the moment
            */
            const bool reserved = (page * MEM_PAGE_SIZE_BYTES) < MEM_VIRTUAL_RESERVED_KERNEL_MEM;

            struct physical_page_specifier_t * entry = &physicalPages[page];
            entry->present = true;
            entry->available = usable;
            entry->free = entry->available && !reserved;
        }
    }

    /*
        Debug log

        Available memory bytes
    */

    char bytesString[11];
    size_t physicalBytes = 0;
    for (size_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        physicalBytes += physicalPages[page].present * MEM_PAGE_SIZE_BYTES;
    }
    memzero(bytesString, 11);
    uint2str(physicalBytes, bytesString, 10);
    LOG2("Installed memory (bytes): ", bytesString);
    size_t availableBytes = 0;
    for (size_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        availableBytes += physicalPages[page].available * MEM_PAGE_SIZE_BYTES;
    }
    memzero(bytesString, 11);
    uint2str(availableBytes, bytesString, 10);
    LOG2("Physical memory available (bytes): ", bytesString);
    size_t freeBytes = k_mem_getFreeBytes();
    memzero(bytesString, 11);
    uint2str(freeBytes, bytesString, 10);
    LOG2("Free memory (bytes): ", bytesString);
}

size_t k_paging_getFreePages() {
    size_t freePages = 0;
    for (size_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        if (physicalPages[page].free) {
            ++freePages;
        }
    }

    return freePages;
}

void k_paging_prepare() {
    /*
        Initialize kernel page tables to map the lower physical memory

        TODO: Initialize as many pages as needed after startup (definetely less than 4MiB)
    */
    for (size_t tableIndex = 0; tableIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++tableIndex) {
        virtual_page_table_t * table = &kernelPageTables[tableIndex];
        
        for (size_t pageIndex = 0; pageIndex < MEM_MAX_PAGE_ENTRY; ++pageIndex) {
            struct virtual_page_specifier_t * page = &(*table)[pageIndex];

            page->frameAddress = (tableIndex << 10) | pageIndex;
            page->present = true;
            page->writable = true;
            page->user = false;
        }
    }

    /*
        Initialize paging info for each process
    */
    for (size_t procId = 0; procId < MAX_PROC; ++procId) {
        /*
            Clear paging info
        */
        memzero(&processPageTables[procId], sizeof(struct process_paging_info_t));

        /*
            Initialize kernel directory entries
        */
        struct virtual_page_specifier_t * kernelDir = processPageTables[procId].pageDirectory.kernel;

        for (size_t dirEntryIndex = 0; dirEntryIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++dirEntryIndex) {
            assignDirEntry(&kernelDir[dirEntryIndex], &kernelPageTables[dirEntryIndex], false);
        }
    }

    /*
        Set up temporary 1:1 mapping
    */
    memcopy(&processPageTables[0].pageDirectory.kernel, 
            &processPageTables[0].pageDirectory, 
            sizeof(processPageTables->pageDirectory.kernel));
}

__attribute__((naked)) void k_paging_start() {
    k_cpu_stackPtr -= MEM_VIRTUAL_RESERVED_KERNEL_MEM;
    extern addr_t k_paging_start_end;
    ptr_t pagingEndJmp = (ptr_t)&k_paging_start_end - MEM_VIRTUAL_RESERVED_KERNEL_MEM;

    __asm__ volatile ("mov %0, %%cr3" : : "a" (&(processPageTables[0].pageDirectory)));
    __asm__ volatile ("mov %cr0, %eax");
    __asm__ volatile ("or $" STR(CR0_PAGING_ENABLE_BIT) ", %eax");
    __asm__ volatile ("mov %eax, %cr0");

    __asm__ volatile ("jmp *%0" : : "r" (pagingEndJmp));
    __builtin_unreachable();
}

void k_paging_init(const struct k_krn_memMapEntry *memMap) {
    if (!memMap) {
        OOPS("Mem map nullptr",);
    }

    /*
        Remove 1:1 mapping
    */
    memzero(&processPageTables[0].pageDirectory,
            sizeof(processPageTables->pageDirectory.kernel));

    /*
        Reload paging tables
    */
    __asm__ volatile ("mov %cr3, %eax");
    __asm__ volatile ("mov %eax, %cr3");

    initializePhysicalMemoryMap(memMap);
}

enum k_mem_error k_paging_assign(const procId_t procId,
                                 const size_t startPage,
                                 const size_t pageCount) {
    if (!k_proc_idIsUser(procId)) {
        return ME_INVALID_ARGUMENT;
    }
    if (!pageCount) {
        return ME_INVALID_ARGUMENT;
    }
    if (startPage >= MEM_MAX_VIRTUAL_PAGE_COUNT ||
        pageCount >= MEM_MAX_VIRTUAL_PAGE_COUNT ||
        (startPage + pageCount) >= MEM_MAX_VIRTUAL_PAGE_COUNT) {
        return ME_INVALID_ARGUMENT;
    }

    bool newPages = false;
    bool overlap = false;
    for (size_t page = startPage; page < startPage + pageCount; ++page) {
        size_t dirIndex = MEM_DIROF(page);
        if (dirIndex >= MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT) {
            return ME_UNAVAILABLE;
        }
        
        struct virtual_page_specifier_t * dirEntry = &processPageTables[procId].pageDirectory.user[dirIndex];

        /*
            Check dir entry
        */
        if (!dirEntry->present) {
            const virtual_page_table_t * pageTable = getFreeTable(procId);
            if (!pageTable) {
                return ME_OUT_OF_PAGES;
            }

            assignDirEntry(dirEntry, pageTable, true);
        } else if (!dirEntry->user) {
            return ME_UNAVAILABLE;
        }

        struct virtual_page_specifier_t * pageEntry = getPageSpecifier(procId, page);
        if (!pageEntry) {
            OOPS("Unexpected page entry nullptr", ME_UNKNOWN);
        }

        /*
            Check page entry
        */
        if (pageEntry->user) {
            overlap = true;
            continue;
        } else if (pageEntry->present) {
            return ME_UNAVAILABLE;
        }

        size_t physicalPage;
        const enum k_mem_error err = getFreePhysicalPage(&physicalPage);
        if (err != ME_NONE) {
            if (err == ME_OUT_OF_MEMORY) {
                return ME_OUT_OF_MEMORY;
            }

            return ME_UNKNOWN;
        }

        assignVirtualPage(pageEntry, physicalPage, true);
        newPages = true;
    }

    return overlap ? newPages ? ME_PART_ALREADY_ASSIGNED : ME_ALREADY_ASSIGNED : ME_UNKNOWN;
}

void k_paging_procCleanup(const procId_t procId) {
    /*
        Free all the pages taken by the process
    */


    // /*
    //     Clear user directory entries
    // */
    // memzero(&processPageTables[procId].pageDirectory.user, sizeof(struct virtual_page_specifier_t) * MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT);

    // /*
    //     Clear user page tables
    // */
    // memzero(&processPageTables[procId].pageTables, sizeof(virtual_page_table_t) * MEM_VIRTUAL_USER_PAGE_TABLE_COUNT);
        
    // /*
    //     Clear user page table info
    // */
    // memzero(&processPageTables[procId].pageTables, sizeof(virtual_page_table_t) * MEM_VIRTUAL_USER_PAGE_TABLE_COUNT);
#warning TODO
}
