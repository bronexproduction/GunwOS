//
//  gunwmem.h
//  GunwOS
//
//  Created by Artur Danielewski on 29.01.2024.
//

#ifndef GUNWOS_GUNWMEM_H
#define GUNWOS_GUNWMEM_H

#include <types.h>
#include <defs.h>

/*
    Memory error codes
*/
enum gnwMemoryError {
    GME_NONE = 0,
    GME_OUT_OF_MEMORY,
    GME_UNKNOWN = -1
};

/*
    Requests additional memory pages to be assigned to the caller process

    Kernel will try to assign a contiguous virtual memory address space of size PAGE_SIZE * pageCount
    Inability to fit such range in any available virtual memory gap will result in out-of-memory error

    Params:
        * pageCount - number of pages requested by the caller process
        * start - pointer to the result start address

    Return: GME_NONE on success, error code otherwise
*/
extern enum gnwMemoryError memplz(const size_t pageCount, addr_t * const start);

/*
    Relinquishes memory page starting at given virtual memory address

    Params:
        * addr - starting address in virtual memory of page to be released

    Return: GME_NONE on success, error code otherwise
*/
extern enum gnwMemoryError memthx(const addr_t addr);

#endif // GUNWOS_GUNWMEM_H
