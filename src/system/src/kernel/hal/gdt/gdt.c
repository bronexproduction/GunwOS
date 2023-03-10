//
//  gdt.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#include <types.h>
#include "gdt.h"
#include <hal/cpu/cpu.h>
#include <hal/mem/mem.h>

#define GDT_CODE_EMPTY (struct k_gdt_codeEntry){ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_DATA_EMPTY (struct k_gdt_dataEntry){ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_TSS_EMPTY  (struct k_gdt_tssEntry) { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#define GDT_LIMIT_H(VAL) ((VAL >> 16) & 0xF)
#define GDT_BASE_H(VAL) (((uint_32)VAL >> 24) & 0xFF)

const struct k_gdt_gdt k_gdt_gdt = { 
    0,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_TSS_EMPTY
};

void k_gdt_init() {

    struct k_gdt_gdt * gdt = (struct k_gdt_gdt *)&k_gdt_gdt;

    /*
        Ring 0 code segment
    */
    gdt->r0code.limitL              = 0xFFFF;
    gdt->r0code.baseL               = 0x0;
    gdt->r0code.accessed            = false;
    gdt->r0code.readable            = true;
    gdt->r0code.conforming          = false;
    gdt->r0code.privilege           = DPL_0;
    gdt->r0code.present             = true;
    gdt->r0code.limitH              = 0xF;
    gdt->r0code.available           = false;
    gdt->r0code.defOperandSize      = OPS_32BIT;
    gdt->r0code.granularity         = GRAN_4K;     
    gdt->r0code.baseH               = 0x0;

    /*
        Ring 0 data segment
    */
    gdt->r0data.limitL              = 0xFFFF;
    gdt->r0data.baseL               = 0x0;
    gdt->r0data.accessed            = false;
    gdt->r0data.writeable           = true;
    gdt->r0data.expandDirection     = EXD_UP;
    gdt->r0data.privilege           = DPL_0;
    gdt->r0data.present             = true;
    gdt->r0data.limitH              = 0xF;
    gdt->r0data.available           = false;
    gdt->r0data.big                 = true;
    gdt->r0data.granularity         = GRAN_4K;
    gdt->r0data.baseH               = 0x0;

    /*
        Ring 3 memory base and limit
    */
    const size_t r3baseBytes = MEM_KERNEL_START + MEM_KERNEL_RESERVED_BYTES;
    const size_t r3limit = (0 - r3baseBytes - MEM_MMIO_RESERVED_BYTES) / KiB(4);

    /*
        Ring 3 code segment
    */
    gdt->r3code.limitL              = r3limit;
    gdt->r3code.baseL               = r3baseBytes;
    gdt->r3code.accessed            = false;
    gdt->r3code.readable            = true;
    gdt->r3code.conforming          = false;
    gdt->r3code.privilege           = DPL_3;
    gdt->r3code.present             = true;
    gdt->r3code.limitH              = GDT_LIMIT_H(r3limit);
    gdt->r3code.available           = false;
    gdt->r3code.defOperandSize      = OPS_32BIT;
    gdt->r3code.granularity         = GRAN_4K;
    gdt->r3code.baseH               = GDT_BASE_H(r3baseBytes);

    /*
        Ring 3 data segment
    */
    gdt->r3data.limitL              = r3limit;
    gdt->r3data.baseL               = r3baseBytes;
    gdt->r3data.accessed            = false;
    gdt->r3data.writeable           = true;
    gdt->r3data.expandDirection     = EXD_UP;
    gdt->r3data.privilege           = DPL_3;
    gdt->r3data.present             = true;
    gdt->r3data.limitH              = GDT_LIMIT_H(r3limit);
    gdt->r3data.available           = false;
    gdt->r3data.big                 = true;
    gdt->r3data.granularity         = GRAN_4K;
    gdt->r3data.baseH               = GDT_BASE_H(r3baseBytes);

    /*
        TSS segment
    */
    gdt->tss.limitL                 = sizeof(k_cpu_tss);
    gdt->tss.baseL                  = (uint_32)&k_cpu_tss;
    gdt->tss.busy                   = false;
    gdt->tss.present                = true;
    gdt->tss.limitH                 = GDT_LIMIT_H(sizeof(k_cpu_tss));
    gdt->tss.granularity            = GRAN_BYTE;
    gdt->tss.baseH                  = GDT_BASE_H(&k_cpu_tss);

    struct __attribute__((packed)) gdt_desc_t {
        uint_16 limit;
        uint_32 base;
    } gdtDesc = {
        sizeof(struct k_gdt_gdt) - 1,
        (uint_32)&k_gdt_gdt
    };

    __asm__ ( "lgdt %0" : : "m"(gdtDesc) );
}
