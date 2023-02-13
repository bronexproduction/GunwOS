//
//  gdt.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#include <stdgunw/types.h>

#include "gdt.h"
#include "../cpu/cpu.h"

#define GDT_CODE_EMPTY (struct k_gdt_codeEntry){ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_DATA_EMPTY (struct k_gdt_dataEntry){ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_TSS_EMPTY  (struct k_gdt_tssEntry) { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

const struct k_gdt_gdt k_gdt_gdt = { 
    0,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_TSS_EMPTY
};

void k_gdt_loadDefault() {

    struct k_gdt_gdt * gdt = (struct k_gdt_gdt *)&k_gdt_gdt;

    gdt->r0code.limitL              = 0xFFFF;
    gdt->r0code.baseL               = 0x0;
    gdt->r0code.accessed            = 0;
    gdt->r0code.readable            = 1;
    gdt->r0code.conforming          = 0;
    gdt->r0code.privilege           = DPL_0;
    gdt->r0code.present             = 1;
    gdt->r0code.limitH              = 0xF;
    gdt->r0code.available           = 0;
    gdt->r0code.defOperandSize32Bit = 1;
    gdt->r0code.granularity4K       = 1;     
    gdt->r0code.baseH               = 0x0;

    gdt->r0data.limitL              = 0xFFFF;
    gdt->r0data.baseL               = 0x0;
    gdt->r0data.accessed            = 0;
    gdt->r0data.writeable           = 1;
    gdt->r0data.expandDown          = 0;
    gdt->r0data.privilege           = DPL_0;
    gdt->r0data.present             = 1;
    gdt->r0data.limitH              = 0xF;
    gdt->r0data.available           = 0;
    gdt->r0data.big                 = 1;
    gdt->r0data.granularity4K       = 1;
    gdt->r0data.baseH               = 0x0;


    gdt->tss.limitL                 = sizeof(k_cpu_tss);
    gdt->tss.baseL                  = (uint_32)&k_cpu_tss;
    gdt->tss.busy                   = 0;
    gdt->tss.present                = 1;
    gdt->tss.limitH                 = (sizeof(k_cpu_tss) >> 16) & 0xF;
    gdt->tss.granularity4K          = 0;
    gdt->tss.baseH                  = ((uint_32)&k_cpu_tss >> 24) & 0xFF;

    struct gdtDesc {
        uint_16 limit;
        uint_32 base;
    } gdtDesc = {
        sizeof(struct k_gdt_gdt),
        (uint_32)&k_gdt_gdt
    };
}
