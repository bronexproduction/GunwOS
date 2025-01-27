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
#define MEM_CONV_LTP(REL)                               (((addr_t)REL) + MEM_VIRTUAL_RESERVED_KERNEL_MEM)

#define MEM_UMA_START                                   KiB(640)
#define MEM_UMA_RESERVED_START                          KiB(768)
#define MEM_UMA_USABLE_SIZE                             (MEM_UMA_RESERVED_START - MEM_UMA_START)
#define MEM_XMS_START                                   MiB(1)

#define _MEM_VALIDATE_VPTR_BUFFER(PROC_ID, VPTR, SIZE_BYTES, RETVAL, ON_ERROR, NULLABLE) {                                                      \
    if (!NULLABLE || VPTR || SIZE_BYTES) {                                                                                                      \
        if (!SIZE_BYTES) { { ON_ERROR; } OOPS("Unexpected buffer size", RETVAL); }                                                              \
        if (!VPTR) { { ON_ERROR; } OOPS("Unexpected null pointer", RETVAL); }                                                                   \
        if (!k_mem_bufferZoneValidForProc(PROC_ID, (ptr_t)VPTR, SIZE_BYTES)) { { ON_ERROR; } OOPS("Reserved zone access violation", RETVAL); }  \
    }                                                                                                                                           \
}
#define MEM_VALIDATE_VPTR_BUFFER(PROC_ID, VPTR, SIZE_BYTES, RETVAL, ON_ERROR) \
    _MEM_VALIDATE_VPTR_BUFFER(PROC_ID, VPTR, SIZE_BYTES, RETVAL, ON_ERROR, false)
#define MEM_VALIDATE_VPTR_BUFFER_NULLABLE(PROC_ID, VPTR, SIZE_BYTES, RETVAL, ON_ERROR) \
    _MEM_VALIDATE_VPTR_BUFFER(PROC_ID, VPTR, SIZE_BYTES, RETVAL, ON_ERROR, true)
#define MEM_VALIDATE_VPTR_RE(PROC_ID, VPTR, TYPE, RETVAL, ON_ERROR) \
    MEM_VALIDATE_VPTR_BUFFER(PROC_ID, VPTR, sizeof(TYPE), RETVAL, ON_ERROR)
#define MEM_VALIDATE_VPTR_R(PROC_ID, VPTR, TYPE, RETVAL) \
    MEM_VALIDATE_VPTR_RE(PROC_ID, VPTR, TYPE, RETVAL,)
#define MEM_VALIDATE_VPTR_E(PROC_ID, VPTR, TYPE, ON_ERROR) \
    MEM_VALIDATE_VPTR_RE(PROC_ID, VPTR, TYPE,, ON_ERROR)
#define MEM_VALIDATE_VPTR(PROC_ID, VPTR, TYPE) \
    MEM_VALIDATE_VPTR_RE(PROC_ID, VPTR, TYPE,,)

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
    Returns linear memory address
    of the physical pointer (kernel only)
*/
ptr_t k_mem_physicalToLinear(const ptr_t physAddr);

/*
    Return if buffer defined by physical address and size is in non-reserved UMA range
*/
bool k_mem_bufferIsInUsableUmaRange(const addr_t physAddress, const size_t size);

/*
    Returns the amount of free (allocable) memory (aligned to pages)
*/
size_t k_mem_getFreeBytes();

/*
    Checks if buffer pointer does not reach
    inaccessible virtual memory areas
*/
bool k_mem_bufferZoneValidForProc(const procId_t procId,
                                  const ptr_t bufferPtr,
                                  const size_t bufferSizeBytes);

/*
    Reserves memory for given process pointed at by vPtr
*/
enum k_mem_error k_mem_gimme(const procId_t procId,
                             const ptr_t vPtr,
                             const ptr_t pPtr,
                             const size_t sizeBytes);

/*
    Releases memory page from given process
*/
enum k_mem_error k_mem_thanksPage(const procId_t procId,
                                  const size_t page);

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
