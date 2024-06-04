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

/*__attribute__((deprecated))*/ struct k_mem_zone {
    ptr_t startPtr;
    ptr_t endPtr;
    size_t sizeBytes;
};

/*__attribute__((deprecated))*/ static struct k_mem_zone zoneForProc(const procId_t procId) {
    struct k_proc_process proc = k_proc_getInfo(procId);
    struct k_mem_zone result;
    memzero(&result, sizeof(struct k_mem_zone));

    if (proc.state == PS_NONE) {
        OOPS("Invalid process state", result);
    }
    
    switch (proc.dpl) { 
        case DPL_0:
            result.startPtr = GDT_SEGMENT_START(r0data);
            result.endPtr = GDT_SEGMENT_END(r0data);
            result.sizeBytes = GDT_LIMIT_BYTES(r0data);
            break;
        case DPL_3:
            result.startPtr = GDT_SEGMENT_START(r3data);
            result.endPtr = GDT_SEGMENT_END(r3data);
            result.sizeBytes = GDT_LIMIT_BYTES(r3data);
            break;
        default:
            break;
    }

    return result;
}

/*__attribute__((deprecated))*/ ptr_t k_mem_absForProc(const procId_t procId, const ptr_t relPtr) {
    if (!relPtr) {
        OOPS("Invalid pointer", nullptr);
    }

    return relPtr + (addr_t)zoneForProc(procId).startPtr;
}

/*__attribute__((deprecated))*/ bool k_mem_bufInZoneForProc(const procId_t procId, const ptr_t absPtr, const size_t bufSize) {
    // check buffer bounds
    // there might be a better way to do it
    // but will do, at least until virtual memory gets implemented
    const struct k_mem_zone procZone = zoneForProc(procId);
    if (absPtr < procZone.startPtr || procZone.endPtr < absPtr) {
        return false;
    }
    
    const size_t trailingBytes = procZone.endPtr - absPtr + 1;
    if (bufSize > trailingBytes) {
        return false;
    }

    return true;
}

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

enum k_mem_error k_mem_zero(const procId_t procId,
                            const ptr_t vPtr,
                            const size_t sizeBytes) {
#warning TODO
    return ME_UNKNOWN;
}

enum k_mem_error k_mem_copy(const procId_t srcProcId,
                            const ptr_t srcVPtr,
                            const procId_t dstProcId,
                            const ptr_t dstVPtr,
                            const size_t sizeBytes) {
#warning TODO
    return ME_UNKNOWN;
}

void k_mem_procCleanup(const procId_t procId) {
    k_paging_procCleanup(procId);
}
