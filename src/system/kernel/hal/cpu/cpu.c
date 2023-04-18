//
//  cpu.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#include "cpu.h"
#include <mem.h>
#include <hal/gdt/gdt.h>

struct k_cpu_tss k_cpu_tss;

void k_cpu_init() {
#warning TODO: CPU configuration? seen on http://www.brokenthorn.com/Resources/OSDev20.html

    memzero(&k_cpu_tss, sizeof(struct k_cpu_tss));

    k_cpu_tss.ss0 = (uint_16)GDT_OFFSET(r0data);
}

void k_cpu_loadTaskRegister() {
    __asm__ volatile ("movw %0, %%ax" : : "r" ((uint_16)(GDT_OFFSET(tss) | DPL_0)));
    __asm__ volatile ("ltr %ax");
}
