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
#include <hal/criticalsec/criticalsec.h>

#include <string.h>
#include <log/log.h>

static physical_page_table_t physicalPages;
static process_page_tables_t processPageTables;
static __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) struct process_paging_info_t kernelPagingInfo;

/*
    Page table to be loaded at startup
*/
static struct process_paging_info_t * const initialPagingInfo = &kernelPagingInfo;

static void getFreeTable(const procId_t procId,
                         const virtual_page_table_t * * const pageTable,
                         struct virtual_page_table_specifier_t * * const pageTableSpecifier) {
    if (!pageTable || !pageTableSpecifier) {
        OOPS("Nullptr",);
    }
    if (!k_proc_idIsUser(procId)) {
        OOPS("Illegal process ID",);
    }

    for (size_t table = 0; table < MEM_VIRTUAL_USER_PAGE_TABLE_COUNT; ++table) {
        if (!processPageTables[procId].pageTableInfo[table].used) {
            *pageTable = &processPageTables[procId].pageTables[table];
            *pageTableSpecifier = &processPageTables[procId].pageTableInfo[table];
            return;
        }
    }

    *pageTable = nullptr;
    *pageTableSpecifier = nullptr;
}

static struct virtual_page_specifier_t * getPageSpecifier(const procId_t procId, const size_t page) {
    if (!k_proc_idIsUser(procId)) {
        OOPS("Illegal process ID", nullptr);
    }
    if (page >= MEM_MAX_VIRTUAL_PAGE_COUNT) {
        OOPS("Illegal page number", nullptr);
    }

    virtual_page_table_t * const pageTable = (virtual_page_table_t *)(processPageTables[procId].pageDirectory.flat[MEM_DIR_INDEX_OF_PAGE(page)].frameAddress << 12);
    if (!pageTable) {
        return nullptr;
    }

    return &(*pageTable)[MEM_TABLE_INDEX_OF_PAGE(page)];
}

static enum k_mem_error getFreePhysicalPage(size_t * const pageIndexPtr) {
    if (!pageIndexPtr) {
        OOPS("Nullptr", ME_UNKNOWN);
    }

    for (size_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        const struct physical_page_specifier_t * const pageInfo = &physicalPages[page];
        if (pageInfo->free) {
            *pageIndexPtr = page;
            return ME_NONE;
        }
    }

    return ME_OUT_OF_MEMORY;
}

static void unsafe_assignVirtualPage(struct virtual_page_specifier_t * const pageEntryPtr,
                                     const size_t physicalPageIndex,
                                     const bool user) {
    pageEntryPtr->frameAddress = physicalPageIndex;
    pageEntryPtr->present = true;
    pageEntryPtr->writable = true;
    pageEntryPtr->user = user;
}

static void assignVirtualPage(struct virtual_page_specifier_t * const pageEntryPtr,
                              const size_t physicalPageIndex,
                              const bool user) {
    if (!pageEntryPtr) {
        OOPS("Nullptr",);
    }
    if (physicalPageIndex >= MEM_MAX_PHYSICAL_PAGE_COUNT) {
        OOPS("Invalid physical page index",);
    }

    struct physical_page_specifier_t * physicalPageSpecifier = &physicalPages[physicalPageIndex];

    if (!physicalPageSpecifier->present) {
        OOPS("Page index outside physical memory",);
    }
    if (user) {
        if (!physicalPageSpecifier->available) {
            OOPS("Physical page unavailable",);
        }
        if (!physicalPageSpecifier->free) {
            OOPS("Physical page already assigned",);
        }
    }

    physicalPageSpecifier->free = false;
    unsafe_assignVirtualPage(pageEntryPtr, physicalPageIndex, user);
}

static void assignDirEntry(struct virtual_page_specifier_t * const dirEntryPtr,
                           const virtual_page_table_t * const physicalPageTablePtr,
                           struct virtual_page_table_specifier_t * const pageTableInfoPtr,
                           const bool user) {
    if (!dirEntryPtr || !physicalPageTablePtr) {
        OOPS("Nullptr",);
    }

    addr_t frameAddr = ((addr_t)physicalPageTablePtr) >> 12;
    if ((frameAddr << 12) != (addr_t)physicalPageTablePtr) {
        OOPS("Invalid page table alignment",);
    }

    dirEntryPtr->frameAddress = frameAddr;
    dirEntryPtr->present = true;
    dirEntryPtr->writable = true;
    dirEntryPtr->user = user;

    if (pageTableInfoPtr) {
        pageTableInfoPtr->used = true;
    }
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

static void unsafe_initializePagingInfo(struct process_paging_info_t * const pagingInfo) {
    /*
        Clear kernel paging info
    */
    memzero(pagingInfo, sizeof(struct process_paging_info_t));

    /*
        Initialize kernel directory entries
    */
    struct virtual_page_specifier_t * dir = pagingInfo->pageDirectory.byArea.kernel;

    for (size_t entryIndex = 0; entryIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++entryIndex) {
        assignDirEntry(&dir[entryIndex], &kernelPagingInfo.pageTables[entryIndex], nullptr, false);
    }
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
        Initialize kernel paging info
    */
    unsafe_initializePagingInfo(&kernelPagingInfo);
    
    /*
        Initialize paging info for each userspace process
    */
    for (size_t procId = 0; procId < MAX_PROC; ++procId) {
        unsafe_initializePagingInfo(&processPageTables[procId]);
    }

    /*
        Mark all kernel pages as kernel only
    */
    for (size_t table = 0; table < MEM_VIRTUAL_USER_PAGE_TABLE_COUNT; ++table) {
        for (size_t page = 0; page < MEM_MAX_PAGE_ENTRY; ++page) {
            kernelPagingInfo.pageTables[table][page].user = false;
        }
    }
    
    /*
        Initialize kernel page tables to map the lower physical memory

        TODO: Initialize as many pages as needed after startup (definetely less than 4MiB)
    */
    for (size_t tableIndex = 0; tableIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++tableIndex) {
        kernelPagingInfo.pageTableInfo[tableIndex].used = true;
        for (size_t pageIndex = 0; pageIndex < MEM_MAX_PAGE_ENTRY; ++pageIndex) {
            unsafe_assignVirtualPage(&kernelPagingInfo.pageTables[tableIndex][pageIndex], (tableIndex << 10) | pageIndex, false);
        }
    }

    /*
        Set up temporary 1:1 mapping
    */
    memcopy(&initialPagingInfo->pageDirectory.byArea.kernel, 
            &initialPagingInfo->pageDirectory, 
            sizeof(initialPagingInfo->pageDirectory.byArea.kernel));
}

__attribute__((naked)) void k_paging_start() {
    k_cpu_stackPtr -= MEM_VIRTUAL_RESERVED_KERNEL_MEM;
    extern addr_t k_paging_start_end;
    ptr_t pagingEndJmp = (ptr_t)&k_paging_start_end - MEM_VIRTUAL_RESERVED_KERNEL_MEM;

    __asm__ volatile ("mov %0, %%cr3" : : "a" (&(initialPagingInfo->pageDirectory)));
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
    memzero(&initialPagingInfo->pageDirectory,
            sizeof(initialPagingInfo->pageDirectory.byArea.kernel));

    /*
        Flush TLB
    */
    k_cpu_tlbFlush();

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
        size_t dirIndex = MEM_DIR_INDEX_OF_PAGE(page);
        if (dirIndex >= MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT) {
            return ME_UNAVAILABLE;
        }
        
        struct virtual_page_specifier_t * dirEntry = &processPageTables[procId].pageDirectory.byArea.user[dirIndex];

        /*
            Check dir entry
        */
        if (!dirEntry->present) {
            const virtual_page_table_t * pageTable;
            struct virtual_page_table_specifier_t * pageTableSpecifier;

            getFreeTable(procId, &pageTable, &pageTableSpecifier);
            if (!pageTable || !pageTableSpecifier) {
                return ME_OUT_OF_PAGES;
            }

            assignDirEntry(dirEntry, (virtual_page_table_t *)MEM_CONV_LTP(pageTable), pageTableSpecifier, true);
        } else if (!dirEntry->user) {
            return ME_UNAVAILABLE;
        }

        struct virtual_page_specifier_t * pageEntry = getPageSpecifier(procId, page);
        if (!pageEntry) {
            OOPS("Unexpected page entry nullptr", ME_UNKNOWN);
        }
        
        pageEntry = (struct virtual_page_specifier_t *)MEM_CONV_PTL(pageEntry);

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

    if (k_cpu_getCR3() == MEM_CONV_LTP(&processPageTables[procId].pageDirectory)) {
        k_cpu_tlbFlush();
    }

    return overlap ? newPages ? ME_PART_ALREADY_ASSIGNED : ME_ALREADY_ASSIGNED : ME_NONE;
}

size_t k_paging_switch(const procId_t procId) {
    if (!k_proc_idIsUser(procId) && procId != KERNEL_PROC_ID) {
        OOPS("Invalid procId", 0);
    }
    
    union virtual_page_directory_t * pageDir = procId == KERNEL_PROC_ID ? &kernelPagingInfo.pageDirectory : &processPageTables[procId].pageDirectory;
    size_t cr3 = k_cpu_getCR3();

    k_cpu_setCR3(MEM_CONV_LTP(pageDir));

    return cr3;
}

void k_paging_procCleanup(const procId_t procId) {
    /*
        Free all the pages taken by the process
    */

    if (!k_proc_idIsUser(procId)) {
        OOPS("Invalid paging cleanup procId",);
    }

    {
        char bytesString[11];
        uint2str(procId, bytesString, 10);
        LOG3("Process ", bytesString, " cleanup");
        size_t freeBytes = k_mem_getFreeBytes();
        memzero(bytesString, 11);
        uint2str(freeBytes, bytesString, 10);
        LOG2("  Free memory (bytes) before: ", bytesString);
    }

    #warning TODO: free physical pages used by process

    memzero(processPageTables[procId].pageDirectory.byArea.user,
     sizeof(processPageTables[procId].pageDirectory.byArea.user));

    memzero(processPageTables[procId].pageTables,
     sizeof(processPageTables[procId].pageTables));

    memzero(processPageTables[procId].pageTableInfo,
     sizeof(processPageTables[procId].pageTableInfo));

    {
        char bytesString[11];
        size_t freeBytes = k_mem_getFreeBytes();
        memzero(bytesString, 11);
        uint2str(freeBytes, bytesString, 10);
        LOG2("  Free memory (bytes) after: ", bytesString);
    }
}
