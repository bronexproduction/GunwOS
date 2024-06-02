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

#define MEM_CONV_PTL(REL)                               (((addr_t)REL) - MEM_VIRTUAL_RESERVED_KERNEL_MEM)

#define MEM_XMS_START                                   MiB(1)

enum k_mem_error {
    ME_NONE = 0,
    ME_PART_ALREADY_ASSIGNED,
    ME_ALREADY_ASSIGNED,
    ME_UNAVAILABLE,
    ME_OUT_OF_PAGES,
    ME_OUT_OF_MEMORY,
    ME_INVALID_ARGUMENT,
    ME_UNKNOWN
};

/*
    DEPRECATED

    Returns absolute memory address 
    of the pointer relative to process accessible space
*/
/*__attribute__((deprecated))*/ ptr_t k_mem_absForProc(const procId_t procId, const ptr_t relPtr);

/*
    DEPRECATED
    
    Checks whether the buffer is inside given process' memory zone
*/
/*__attribute__((deprecated))*/ bool k_mem_bufInZoneForProc(const procId_t procId, const ptr_t absPtr, const size_t bufSize);

/*
    Returns linear memory address
    of the physical pointer (kernel only)
*/
ptr_t k_mem_physicalToLinear(const ptr_t physAddr);

/*
    Returns the amount of free (allocable) memory (aligned to pages)
*/
size_t k_mem_getFreeBytes();

/*
    Reserves memory for given process pointed at by vPtr
*/
enum k_mem_error k_mem_gimme(const procId_t procId,
                             const ptr_t vPtr,
                             const size_t sizeBytes);

/*
    Zeroes memory of given process pointed at by vPtr
*/
enum k_mem_error k_mem_zero(const procId_t procId,
                            const ptr_t vPtr,
                            const size_t sizeBytes);

/*
    Copies virtual memory content between processes
*/
enum k_mem_error k_mem_copy(const procId_t srcProcId,
                            const ptr_t srcVPtr,
                            const procId_t dstProcId,
                            const ptr_t dstVPtr,
                            const size_t sizeBytes);

/*
    Releases all the allocated and reserved memory for given process
*/
void k_mem_procCleanup(const procId_t procId);

#endif // MEM_H
