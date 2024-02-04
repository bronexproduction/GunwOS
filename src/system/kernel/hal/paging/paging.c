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

struct {
    struct __attribute__((packed, aligned(MEM_PAGE_SIZE_BYTES))) {
        /*
            Lower memory addresses (TEMPORARY - to be swapped with user pages)
        */
        struct __attribute__((packed)) {
            struct k_virtual_page_specifier_t kernel[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];
        } reserved;
        /*
            Topmost memory address range (TEMPORARY - to be swapped with kernel pages)
        */
        struct k_virtual_page_specifier_t user[MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT];
    } pageDirectory;
    
    /*
        CHECK ALIGNMENT

        NOTE: Must be 4KiB-aligned due to the PDE addressing scheme
    */
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) k_virtual_page_table_t pageTables[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
} pagingInfo[MAX_PROC];

static __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) k_virtual_page_table_t kernelPageTables[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];

void k_paging_init() {
    /*
        Initialize kernel page tables
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

            Funny info:

            Manipulation
The CR3 value, that is, the value containing the address of the page directory, is in physical form. Once, then, the computer is in paging mode, only recognizing those virtual addresses mapped into the paging tables, how can the tables be edited and dynamically changed?

Many prefer to map the last PDE to itself. The page directory will look like a page table to the system. To get the physical address of any virtual address in the range 0x00000000-0xFFFFF000 is then just a matter of:

void *get_physaddr(void *virtualaddr) {
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));
}
To map a virtual address to a physical address can be done as follows:

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    // Make sure that both addresses are page-aligned.
 
    unsigned long pdindex = (unsigned long)virtualaddr >> 22;
    unsigned long ptindex = (unsigned long)virtualaddr >> 12 & 0x03FF;
 
    unsigned long *pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
    // When it is not present, you need to create a new empty PT and
    // adjust the PDE accordingly.
 
    unsigned long *pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
    // When it is, then there is already a mapping present. What do you do now?
 
    pt[ptindex] = ((unsigned long)physaddr) | (flags & 0xFFF) | 0x01; // Present
 
    // Now you need to flush the entry in the TLB
    // or you might not notice the change.
}
Unmapping an entry is essentially the same as above, but instead of assigning the pt[ptindex] a value, you set it to 0x00000000 (i.e. not present). When the entire page table is empty, you may want to remove it and mark the page directory entry 'not present'. Of course you don't need the 'flags' or 'physaddr' for unmapping.
        */

        for (size_t dirEntryIndex = 0; dirEntryIndex < MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT; ++dirEntryIndex) {
            struct k_virtual_page_specifier_t * dirEntry = &kernelDir[dirEntryIndex];

            dirEntry->frameAddress = (addr_t)&kernelPageTables[dirEntryIndex] >> 12;
            dirEntry->present = true;
            dirEntry->writable = true;
            dirEntry->user = false;
        }
        
        // initalize process table data
    }

    register __attribute__((unused)) addr_t dupa __asm__ ("eax") = (addr_t)&(pagingInfo[0].pageDirectory);
    __asm__ volatile ("mov %eax, %cr3");
    __asm__ volatile ("mov %cr0, %eax");
    __asm__ volatile ("or $" STR(CR0_PAGING_ENABLE_BIT) ", %eax");
    __asm__ volatile ("mov %eax, %cr0");
    __asm__ volatile ("jmp k_paging_init_end");
    __asm__ volatile ("k_paging_init_end:");
}
