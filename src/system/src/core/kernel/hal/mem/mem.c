//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#include "mem.h"
#include "../proc/proc.h"

struct k_mem_zone k_mem_zoneForProc(int_32 procId) {
    enum k_gdt_dpl dpl = k_proc_getInfo(k_proc_getCurrentId()).dpl;
    (void)dpl;

    #warning to be implemented

    return (struct k_mem_zone){0,0};
}
