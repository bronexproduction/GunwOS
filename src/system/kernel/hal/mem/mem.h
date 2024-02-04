//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef MEM_H
#define MEM_H

#include <defs.h>
#include <types.h>

#define MEM_PHYSICAL_ADDRESSABLE_MEM                    MiB(16)
#define MEM_VIRTUAL_RESERVED_KERNEL_MEM                 MiB(4)
typedef struct __attribute__((packed)) k_virtual_page_specifier_t {
    bool present            :1;
    bool writable           :1;
    bool user               :1;
    uint_8 _reserved0       :2;
    bool accessed           :1;
    bool dirty              :1;
    uint_8 _reserved1       :2;
    uint_8 _unused          :3;
    uint_32 frameAddress    :20;
} k_virtual_page_table_t[MEM_MAX_PAGE_ENTRY];

struct __attribute__((packed)) k_virtual_paging_info_t {
    struct __attribute__((packed)) {
        struct k_virtual_page_specifier_t user[MEM_VIRTUAL_USER_MAX_PAGE_TABLE_COUNT];
        const struct __attribute__((packed)) {
            struct k_virtual_page_specifier_t kernel[MEM_VIRTUAL_RESERVED_KERNEL_PAGE_TABLE_COUNT];
        } reserved;
    } pageDirectory;
    k_virtual_page_table_t pageTables[MEM_VIRTUAL_USER_PAGE_TABLE_COUNT];
};

/*
    Returns absolute memory address 
    of the pointer relative to process accessible space
*/
ptr_t k_mem_absForProc(const procId_t procId, const ptr_t relPtr);

/*
    Checks whether the buffer is inside given process' memory zone
*/
bool k_mem_bufInZoneForProc(const procId_t procId, const ptr_t absPtr, const size_t bufSize);

/*
    Releases all the allocated memory for given process
*/
void k_mem_procCleanup(const procId_t procId);

#endif // MEM_H
