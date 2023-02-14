//
//  cpu.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#include "cpu.h"
#include <stdgunw/mem.h>

struct k_cpu_tss k_cpu_tss;

void k_cpu_init() {
#warning TODO: CPU configuration? seen on http://www.brokenthorn.com/Resources/OSDev20.html

    memnull(&k_cpu_tss, sizeof(struct k_cpu_tss));
}
