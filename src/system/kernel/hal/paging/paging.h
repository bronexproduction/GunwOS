//
//  paging.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2024.
//

#ifndef PAGING_H
#define PAGING_H

#include <defs.h>
#include <kernel_boot_data.h>

/*
    Hardware constraints
*/
#define MEM_PAGE_SIZE_BYTES                             KiB(4)

#define MEM_PAGE_OF_ADDR(ADDR)                          ((ADDR) / (MEM_PAGE_SIZE_BYTES))

void k_paging_prepare();
__attribute__((naked)) void k_paging_start();
void k_paging_init(const struct k_krn_memMapEntry *memMap);

/*
    Returns the number of free pages
*/
size_t k_paging_getFreePages();

/*
    Requests memory pages to be assigned to the process
*/
enum k_mem_error k_paging_assign(const procId_t procId,
                                 const size_t startPage,
                                 const size_t pageCount);

/*
    Removes any pages assignment to given process
*/
void k_paging_procCleanup(const procId_t procId);

#endif // PAGING_H
