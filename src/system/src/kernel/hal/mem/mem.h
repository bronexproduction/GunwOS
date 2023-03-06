//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef MEM_H
#define MEM_H

#include <stdgunw/defs.h>
#include <stdgunw/types.h>

#define MEM_XMS_START               MiB(1)
#define MEM_KERNEL_START            MEM_XMS_START
#define MEM_KERNEL_RESERVED_BYTES   MiB(2)
#define MEM_MMIO_RESERVED_BYTES     MiB(256)

struct k_mem_zone {
    ptr_t startPtr;
    ptr_t endPtr;
    size_t sizeBytes;
};

/*
    Returns accessible memory zone limits for given process id
*/
struct k_mem_zone k_mem_zoneForProc(int_32 procId);

#endif // MEM_H
