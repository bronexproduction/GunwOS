//
//  data.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.05.2024.
//

#ifndef DATA_H
#define DATA_H

#include "paging.h"

#include <hal/mem/mem.h>
#include <hal/proc/proc.h>

/*
    Hardware constraints
*/
#define MEM_MAX_DIR_ENTRY                               (1 << 10)
#define MEM_MAX_PAGE_ENTRY                              (1 << 10)
#define MEM_SPACE_PER_DIR_ENTRY                         (MEM_PAGE_SIZE_BYTES * MEM_MAX_PAGE_ENTRY)

/*
    Physical memory
*/
#define MEM_MAX_PHYSICAL_PAGE_COUNT                     (MEM_MAX_DIR_ENTRY * MEM_MAX_PAGE_ENTRY)
#define MEM_PHYSICAL_PAGE_COUNT                         (MEM_PHYSICAL_ADDRESSABLE_MEM / MEM_PAGE_SIZE_BYTES)
#define MEM_PHYSICAL_PAGE_TABLE_COUNT                   (MEM_PHYSICAL_ADDRESSABLE_MEM / MEM_SPACE_PER_DIR_ENTRY)

/*
    Virtual memory
*/
#define MEM_MAX_VIRTUAL_PAGE_COUNT                      (MEM_MAX_PHYSICAL_PAGE_COUNT)
#define MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT    (MEM_VIRTUAL_RESERVED_KERNEL_MEM / MEM_SPACE_PER_DIR_ENTRY)
#define MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT           (MEM_MAX_DIR_ENTRY - MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT)
#define MEM_VIRTUAL_USER_PAGE_TABLE_COUNT               MEM_PHYSICAL_PAGE_TABLE_COUNT

/*
    Helper macros
*/
#define MEM_DIR_INDEX_OF_PAGE(PAGE)                     ((PAGE) / (MEM_MAX_PAGE_ENTRY))
#define MEM_TABLE_INDEX_OF_PAGE(PAGE)                   ((PAGE) % (MEM_MAX_PAGE_ENTRY))

_Static_assert(!(MEM_PHYSICAL_ADDRESSABLE_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_PHYSICAL_ADDRESSABLE_MEM not aligned to addressable space unit");
_Static_assert(!(MEM_VIRTUAL_RESERVED_KERNEL_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_VIRTUAL_RESERVED_KERNEL_MEM not aligned to addressable space unit");
_Static_assert(MEM_PHYSICAL_PAGE_TABLE_COUNT <= MEM_MAX_DIR_ENTRY, "MEM_PHYSICAL_PAGE_TABLE_COUNT exceeds MEM_MAX_DIR_ENTRY");
_Static_assert(MEM_PAGE_SIZE_BYTES >= 2, "MEM_PAGE_SIZE_BYTES cannot be less than 2");
_Static_assert(MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT + MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT == MEM_MAX_DIR_ENTRY, "User and reserved pages count not equal MEM_MAX_VIRTUAL_PAGE_COUNT");
_Static_assert(MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT <= MEM_VIRTUAL_USER_PAGE_TABLE_COUNT, "Kernel reserved pages count exceeds MEM_VIRTUAL_USER_PAGE_TABLE_COUNT");

#define CR0_PAGING_ENABLE_BIT 0x80000000

typedef struct __attribute__((packed)) virtual_page_specifier_t {
    bool present            :1;
    bool writable           :1;
    bool user               :1;
    uint_8 _reserved1       :2;
    bool accessed           :1;
    bool dirty              :1;
    uint_8 _reserved0       :2;
    uint_8 _unused          :3;
    uint_32 frameAddress    :20;
} virtual_page_table_t[MEM_MAX_PAGE_ENTRY];

struct __attribute__((packed)) virtual_page_table_specifier_t {
    bool used               :1;
};

typedef struct __attribute__((packed)) physical_page_specifier_t {
    bool present    :1; // Installed physical RAM
    bool available  :1; // Available (non-reserved) physical RAM
    bool free       :1; // Free
} physical_page_table_t[MEM_PHYSICAL_PAGE_COUNT];

union __attribute__((packed)) virtual_page_directory_t {
    struct __attribute__((packed)) {
        struct virtual_page_specifier_t user[MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT];
        struct virtual_page_specifier_t kernel[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];
    } byArea;
    struct virtual_page_specifier_t flat[MEM_MAX_DIR_ENTRY];
};

typedef struct __attribute__((packed)) process_paging_info_t {
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) union virtual_page_directory_t pageDirectory;
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) virtual_page_table_t pageTables[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
    __attribute__((aligned(MEM_PAGE_SIZE_BYTES))) struct virtual_page_table_specifier_t pageTableInfo[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
} process_page_tables_t[MAX_PROC];

#endif // DATA_H
