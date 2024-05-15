//
//  paging.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2024.
//

#ifndef PAGING_H
#define PAGING_H

#include <kernel_boot_data.h>

void k_paging_prepare();
__attribute__((naked)) void k_paging_start();
void k_paging_init(const struct k_krn_memMapEntry *memMap);

#endif // PAGING_H
