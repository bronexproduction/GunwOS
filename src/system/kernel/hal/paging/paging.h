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

void k_paging_prepare();
__attribute__((naked)) void k_paging_start();
void k_paging_init(const struct k_krn_memMapEntry *memMap);

/*
    Returns the number of free pages
*/
size_t k_paging_getFreePages();

#endif // PAGING_H
