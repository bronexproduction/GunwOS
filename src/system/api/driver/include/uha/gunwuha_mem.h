//
//  gunwuha_mem.h
//  Universal Hardware Abstraction for memory-mapped devices
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2023.
//

#ifndef GUNWOS_GUNWUHA_MEM_H
#define GUNWOS_GUNWUHA_MEM_H

#include <uha/gunwuha_mem_desc.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_MEM GNW_UHA_EMPTY(MEM)
#define GNW_UHA_MEM_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

struct gnwDeviceUHA_mem_routine {
    /*
        Update the memory
    
        Params:
            * Buffer pointer (see struct gnwDeviceUHA_mem_desc for parameters)
            * Range of the buffer, relative to (0 - desc.maxInputSizeBytes)
    */
    void (*write)(const ptr_t buffer, const range_addr_t inputBufferRange);
};

struct gnwDeviceUHA_mem {
    struct gnwDeviceUHA_mem_desc desc;
    struct gnwDeviceUHA_mem_routine routine;
};

#endif // GUNWOS_GUNWUHA_MEM_H