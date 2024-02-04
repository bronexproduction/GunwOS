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
