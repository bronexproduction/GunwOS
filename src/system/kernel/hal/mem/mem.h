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

#define MEM_XMS_START                   MiB(1)
#define MEM_KERNEL_XMS_START            MEM_XMS_START
#define MEM_KERNEL_XMS_RESERVED_BYTES   MiB(2)
#define MEM_MMIO_RESERVED_BYTES         MiB(256)

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
