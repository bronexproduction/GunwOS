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

#define MEM_PAGE_SIZE_BYTES             KiB(4)
#define MEM_MAX_DIR_ENTRY               (1 << 10)
#define MEM_MAX_PAGE_ENTRY              (1 << 10)
#define MEM_SPACE_PER_DIR_ENTRY         (MEM_PAGE_SIZE_BYTES * MEM_MAX_PAGE_ENTRY)

#define MEM_ADDRESSABLE_PHYSICAL_MEM    MiB(16)
#define MEM_PHYSICAL_PAGE_TABLE_COUNT   (MEM_ADDRESSABLE_PHYSICAL_MEM / MEM_SPACE_PER_DIR_ENTRY)

#define MEM_RESERVED_VIRTUAL_KERNEL_MEM MiB(4)
#define MEM_RESERVED_VIRTUAL_MEM        (MEM_RESERVED_VIRTUAL_KERNEL_MEM)

_Static_assert(!(MEM_ADDRESSABLE_PHYSICAL_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_ADDRESSABLE_PHYSICAL_MEM not aligned to addressable space unit");
_Static_assert(!(MEM_RESERVED_VIRTUAL_MEM % MEM_SPACE_PER_DIR_ENTRY), "MEM_RESERVED_VIRTUAL_MEM not aligned to addressable space unit");
_Static_assert(MEM_PHYSICAL_PAGE_TABLE_COUNT <= MEM_MAX_DIR_ENTRY, "MEM_PHYSICAL_PAGE_TABLE_COUNT exceeds MEM_MAX_DIR_ENTRY");

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
