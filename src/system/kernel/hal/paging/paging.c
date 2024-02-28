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

typedef struct __attribute__((packed)) k_virtual_page_specifier_t {
    bool present            :1;
    bool writable           :1;
    bool user               :1;
    uint_8 _reserved1       :2;
    bool accessed           :1;
    bool dirty              :1;
    uint_8 _reserved0       :2;
    uint_8 _unused          :3;
    uint_32 frameAddress    :20;
} k_virtual_page_table_t[MEM_MAX_PAGE_ENTRY];

struct __attribute__((packed)) k_virtual_pages_reserved_t {
    struct k_virtual_page_specifier_t kernel[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];
} reserved;

struct {
    struct __attribute__((packed, aligned(MEM_PAGE_SIZE_BYTES))) {
        struct k_virtual_page_specifier_t user[MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT];
        struct k_virtual_pages_reserved_t reserved;
    } pageDirectory;
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) k_virtual_page_table_t pageTables[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
} pagingInfo[MAX_PROC];

static __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) k_virtual_page_table_t kernelPageTables[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];

void k_paging_prepare() {
    /*
        Initialize kernel page tables to map the lower physical memory

        TODO: Initialize as many pages as needed after startup (definetely less than 4MiB)
    */
    for (size_t tableIndex = 0; tableIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++tableIndex) {
        k_virtual_page_table_t * table = &kernelPageTables[tableIndex];
        
        for (size_t pageIndex = 0; pageIndex < MEM_MAX_PAGE_ENTRY; ++pageIndex) {
            struct k_virtual_page_specifier_t * page = &(*table)[pageIndex];

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
        struct k_virtual_page_specifier_t * kernelDir = pagingInfo[proc].pageDirectory.reserved.kernel;

        /*
            Clear user pages
        */

        for (size_t dirEntryIndex = 0; dirEntryIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++dirEntryIndex) {
            struct k_virtual_page_specifier_t * dirEntry = &kernelDir[dirEntryIndex];

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
            sizeof(struct k_virtual_pages_reserved_t));
}

__attribute__((naked)) void k_paging_start() {
    k_cpu_stackPtr -= MEM_VIRTUAL_RESERVED_KERNEL_MEM;
    extern addr_t k_paging_start_end;
    ptr_t pagingEndJmp = (ptr_t)&k_paging_start_end - MEM_VIRTUAL_RESERVED_KERNEL_MEM;

    __asm__ volatile ("mov %0, %%cr3" : : "a" (&(pagingInfo[0].pageDirectory)));
    __asm__ volatile ("mov %cr0, %eax");
    __asm__ volatile ("or $" STR(CR0_PAGING_ENABLE_BIT) ", %eax");
    __asm__ volatile ("mov %eax, %cr0");

    __asm__ volatile ("jmp %0" : : "r" (pagingEndJmp));
    __builtin_unreachable();
}

void k_paging_init() {

    /*
        Remove 1:1 mapping
    */
    memzero(&pagingInfo[0].pageDirectory,
            sizeof(struct k_virtual_pages_reserved_t));
    
    /*
        Reload paging tables
    */
    __asm__ volatile ("mov %cr3, %eax");
    __asm__ volatile ("mov %eax, %cr3");
}
