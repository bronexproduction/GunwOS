//
//  gdt.c
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#include <stdgunw/types.h>

#include "gdt.h"

#define GDT_CODE_EMPTY (struct k_gdt_codeEntry){ 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_DATA_EMPTY (struct k_gdt_dataEntry){ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }
#define GDT_TSS_EMPTY  (struct k_gdt_tssEntry) { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

const struct k_gdt_gdt k_gdt_gdt = { 
    0,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_CODE_EMPTY,
    GDT_DATA_EMPTY,
    GDT_TSS_EMPTY
};

void k_gdt_loadDefault() {

}
