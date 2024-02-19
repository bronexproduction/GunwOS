//
//  paging.h
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2024.
//

#ifndef PAGING_H
#define PAGING_H

void k_paging_init();
__attribute__((naked)) void k_paging_start();

#endif // PAGING_H
