//
//  paging.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2024.
//

#include "paging.h"

#include <defs.h>
#include <types.h>
#include <utils.h>
#include <mem.h>
#include <hal/cpu/cpu.h>
#include <hal/mem/mem.h>
#include <hal/proc/proc.h>
#include <error/panic.h>

#include <string.h>
#include <log/log.h>

/*
    Hardware constraints
*/
#define MEM_PAGE_SIZE_BYTES                             KiB(4)
#define MEM_MAX_DIR_ENTRY                               (1 << 10)
#define MEM_MAX_PAGE_ENTRY                              (1 << 10)
#define MEM_SPACE_PER_DIR_ENTRY                         (MEM_PAGE_SIZE_BYTES * MEM_MAX_PAGE_ENTRY)

/*
    Physical memory
*/
#define MEM_MAX_PHYSICAL_PAGE_COUNT                     (MEM_MAX_DIR_ENTRY * MEM_MAX_PAGE_ENTRY)
#define MEM_PHYSICAL_PAGE_COUNT                         (MEM_PHYSICAL_ADDRESSABLE_MEM / MEM_PAGE_SIZE_BYTES)
#define MEM_PHYSICAL_PAGE_TABLE_COUNT                   (MEM_PHYSICAL_ADDRESSABLE_MEM / MEM_SPACE_PER_DIR_ENTRY)

/*
    Virtual memory
*/
#define MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT    (MEM_VIRTUAL_RESERVED_KERNEL_MEM / MEM_SPACE_PER_DIR_ENTRY)
#define MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT           (MEM_MAX_DIR_ENTRY - MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT)
#define MEM_VIRTUAL_USER_PAGE_TABLE_COUNT               MEM_PHYSICAL_PAGE_TABLE_COUNT

_Static_assert(!(MEM_PHYSICAL_ADDRESSABLE_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_PHYSICAL_ADDRESSABLE_MEM not aligned to addressable space unit");
_Static_assert(!(MEM_VIRTUAL_RESERVED_KERNEL_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_VIRTUAL_RESERVED_KERNEL_MEM not aligned to addressable space unit");
_Static_assert(MEM_PHYSICAL_PAGE_TABLE_COUNT <= MEM_MAX_DIR_ENTRY, "MEM_PHYSICAL_PAGE_TABLE_COUNT exceeds MEM_MAX_DIR_ENTRY");

#define CR0_PAGING_ENABLE_BIT 0x80000000

typedef struct __attribute__((packed)) virtual_page_specifier_t {
    bool present            :1;
    bool writable           :1;
    bool user               :1;
    uint_8 _reserved1       :2;
    bool accessed           :1;
    bool dirty              :1;
    uint_8 _reserved0       :2;
    uint_8 _unused          :3;
    uint_32 frameAddress    :20;
} virtual_page_table_t[MEM_MAX_PAGE_ENTRY];

struct __attribute__((packed)) virtual_pages_reserved_t {
    struct virtual_page_specifier_t kernel[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];
} reserved;

struct {
    struct __attribute__((packed, aligned(MEM_PAGE_SIZE_BYTES))) {
        struct virtual_page_specifier_t user[MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT];
        struct virtual_pages_reserved_t reserved;
    } pageDirectory;
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) virtual_page_table_t pageTables[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
} pagingInfo[MAX_PROC];

static __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) virtual_page_table_t kernelPageTables[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];

struct __attribute__((packed)) physical_page_info_t {
    bool present    :1; // Installed physical RAM
    bool available  :1; // Available (non-reserved) physical RAM
    bool free       :1; // Free
} physicalPages[MEM_PHYSICAL_PAGE_COUNT];

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
        Initialize paging info
    */
    for (size_t proc = 0; proc < MAX_PROC; ++proc) {
        struct virtual_page_specifier_t * kernelDir = pagingInfo[proc].pageDirectory.reserved.kernel;

        /*
            Clear user pages
        */

        for (size_t dirEntryIndex = 0; dirEntryIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++dirEntryIndex) {
            struct virtual_page_specifier_t * dirEntry = &kernelDir[dirEntryIndex];

            dirEntry->frameAddress = (addr_t)&kernelPageTables[dirEntryIndex] >> 12;
            dirEntry->present = true;
            dirEntry->writable = true;
            dirEntry->user = false;
        }
    }

    /*
        Temporary 1:1 mapping
    */
    memcopy(&pagingInfo[0].pageDirectory.reserved.kernel, 
            &pagingInfo[0].pageDirectory, 
            sizeof(struct virtual_pages_reserved_t));
}

__attribute__((naked)) void k_paging_start() {
    k_cpu_stackPtr -= MEM_VIRTUAL_RESERVED_KERNEL_MEM;
    extern addr_t k_paging_start_end;
    ptr_t pagingEndJmp = (ptr_t)&k_paging_start_end - MEM_VIRTUAL_RESERVED_KERNEL_MEM;

    __asm__ volatile ("mov %0, %%cr3" : : "a" (&(pagingInfo[0].pageDirectory)));
    __asm__ volatile ("mov %cr0, %eax");
    __asm__ volatile ("or $" STR(CR0_PAGING_ENABLE_BIT) ", %eax");
    __asm__ volatile ("mov %eax, %cr0");

    __asm__ volatile ("jmp *%0" : : "r" (pagingEndJmp));
    __builtin_unreachable();
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

            struct physical_page_info_t * entry = &physicalPages[page];
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
    for (hugeSize_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        physicalBytes += physicalPages[page].present * MEM_PAGE_SIZE_BYTES;
    }
    memzero(bytesString, 11);
    uint2str(physicalBytes, bytesString, 10);
    LOG2("Installed memory (bytes): ", bytesString);
    size_t availableBytes = 0;
    for (hugeSize_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        availableBytes += physicalPages[page].available * MEM_PAGE_SIZE_BYTES;
    }
    memzero(bytesString, 11);
    uint2str(availableBytes, bytesString, 10);
    LOG2("Physical memory available (bytes): ", bytesString);
    size_t freeBytes = 0;
    for (hugeSize_t page = 0; page < MEM_PHYSICAL_PAGE_COUNT; ++page) {
        freeBytes += physicalPages[page].free * MEM_PAGE_SIZE_BYTES;
    }
    memzero(bytesString, 11);
    uint2str(freeBytes, bytesString, 10);
    LOG2("Free memory (bytes): ", bytesString);
}

void k_paging_init(const struct k_krn_memMapEntry *memMap) {
    if (!memMap) {
        OOPS("Mem map nullptr",);
    }

    /*
        Remove 1:1 mapping
    */
    memzero(&pagingInfo[0].pageDirectory,
            sizeof(struct virtual_pages_reserved_t));

    /*
        Reload paging tables
    */
    __asm__ volatile ("mov %cr3, %eax");
    __asm__ volatile ("mov %eax, %cr3");

    initializePhysicalMemoryMap(memMap);
}
