//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#include "mem.h"
#include <mem.h>
#include <hal/proc/proc.h>
#include <hal/paging/paging.h>
#include <error/panic.h>

void k_mem_init() {
}

ptr_t k_mem_physicalToLinear(const ptr_t physAddr) {
    if ((addr_t)physAddr >= MEM_VIRTUAL_RESERVED_KERNEL_MEM) {
        OOPS("Kernel address out of range", nullptr);
    }
    return (ptr_t)(MEM_CONV_PTL(physAddr));
}

size_t k_mem_getFreeBytes() {
    return k_paging_getFreePages() * MEM_PAGE_SIZE_BYTES;
}

bool k_mem_bufferZoneValidForProc(const procId_t procId,
                                  const ptr_t bufferPtr,
                                  const size_t bufferSizeBytes) {
    if (!bufferSizeBytes) {
        return true;
    }
    if (!bufferPtr) {
        return false;
    }
    if (procId == KERNEL_PROC_ID) {
        return true;
    }
    if (!k_proc_idIsUser(procId)) {
        OOPS("Invalid process identifier", false);
    }

    ptr_t bufferEndPtr = bufferPtr + bufferSizeBytes - 1;
    if (bufferEndPtr < bufferPtr) {
        return false;
    }
    if (bufferEndPtr >= (ptr_t)(0 - MEM_VIRTUAL_RESERVED_KERNEL_MEM)) {
        return false;
    }

    return true;
}

enum k_mem_error k_mem_gimme(const procId_t procId,
                             const ptr_t vPtr,
                             const size_t sizeBytes) {
    if (!k_proc_idIsUser(procId)) {
        return ME_INVALID_ARGUMENT;
    }

    ptr_t vEnd = vPtr + sizeBytes;
    if (vEnd <= vPtr) {
        return ME_INVALID_ARGUMENT;
    }
    if ((addr_t)vEnd > ((addr_t)0 - MEM_VIRTUAL_RESERVED_KERNEL_MEM)) {
        return ME_INVALID_ARGUMENT;
    }

    size_t startPage = MEM_PAGE_OF_ADDR((addr_t)vPtr);
    size_t pageCount = MEM_PAGE_OF_ADDR((addr_t)aligned((addr_t)vEnd, MEM_PAGE_SIZE_BYTES)) - startPage;

    return k_paging_assign(procId, startPage, pageCount);
}

enum k_mem_error k_mem_thanksPage(const procId_t procId,
                                  const size_t page) {
    if (!k_proc_idIsUser(procId)) {
        return ME_INVALID_ARGUMENT;
    }
    if (!page) {
        return ME_INVALID_ARGUMENT;
    }

    return k_paging_release(procId, page);
}

enum k_mem_error k_mem_zero(const procId_t procId,
                            const ptr_t vPtr,
                            const size_t sizeBytes) {
    if (!k_proc_idIsUser(procId)) {
        return ME_INVALID_ARGUMENT;
    }
    
    MEM_ONTABLE(procId, {
        memzero(vPtr, sizeBytes);
    });

    return ME_NONE;
}

enum k_mem_error k_mem_copy(const procId_t srcProcId,
                            const ptr_t srcVPtr,
                            const procId_t dstProcId,
                            const ptr_t dstVPtr,
                            const size_t sizeBytes) {

    if (!srcVPtr || !dstVPtr) {
        OOPS("Nullptr", ME_INVALID_ARGUMENT);
    }
    if (srcProcId != KERNEL_PROC_ID && !k_proc_idIsUser(srcProcId)) {
        OOPS("Invalid source process ID", ME_INVALID_ARGUMENT);
    }
    if (dstProcId != KERNEL_PROC_ID && !k_proc_idIsUser(dstProcId)) {
        OOPS("Invalid destination process ID", ME_INVALID_ARGUMENT);
    }

    const procId_t tableProcId = (srcProcId != KERNEL_PROC_ID && dstProcId != KERNEL_PROC_ID) ? NONE_PROC_ID : (srcProcId == KERNEL_PROC_ID) ? dstProcId : srcProcId; 
    if (tableProcId == NONE_PROC_ID) {
        uint_8 byteBuffer;
        for (size_t byteIndex = 0; byteIndex < sizeBytes; ++byteIndex) {
            MEM_ONTABLE(srcProcId, {
                byteBuffer = *((uint_8 *)srcVPtr + byteIndex);
            });
            MEM_ONTABLE(dstProcId, {
                *((uint_8 *)dstVPtr + byteIndex) = byteBuffer;
            });
        }
    } else {
        MEM_ONTABLE(tableProcId, {
            memcopy(srcVPtr, dstVPtr, sizeBytes);
        });
    }

    return ME_NONE;
}

void k_mem_procCleanup(const procId_t procId) {
    k_paging_procCleanup(procId);
}
