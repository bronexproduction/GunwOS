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
    Allocates and returns a contiguous memory space of 'sizeBytes' bytes in size

    Params:
        * sizeBytes - size of the requested memory space

    Return: Pointer to the newly allocated memory or nullptr if anything went wrong
*/
extern ptr_t memPlz(const size_t sizeBytes);

/*
    Releases previously allocated memory region

    Params:
        * ptr - pointer to already allocated region
*/
extern void memThx(const ptr_t ptr);

/*
    Requests additional memory pages to be assigned to the caller process

    Kernel will try to assign a contiguous virtual memory address space of size PAGE_SIZE * pageCount
    Inability to fit such range in any available virtual memory gap will result in out-of-memory error

    Params:
        * pageCount - number of pages requested by the caller process
        * start - pointer to the result start address

    Return: GME_NONE on success, error code otherwise
*/
extern enum gnwMemoryError memPagePlz(const size_t pageCount, const addr_t start);

/*
    Relinquishes memory page starting at given virtual memory address

    Params:
        * addr - starting address in virtual memory of page to be released

    Return: GME_NONE on success, error code otherwise
*/
extern enum gnwMemoryError memPageThx(const addr_t addr);

#endif // GUNWOS_GUNWMEM_H
