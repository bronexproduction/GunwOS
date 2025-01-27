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
#include <gunwdevtypes.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_MEM GNW_UHA_EMPTY(MEM)
#define GNW_UHA_MEM_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

typedef void (*gnwDeviceMemWriteQueryDecoder)(const ptr_t, struct gnwDeviceMemWriteQuery * const);

struct gnwDeviceUHA_mem_routine {
    /*
        Update the memory
    
        Params:
            * queryPtr - pointer to struct gnwDeviceMemWriteQuery

        It's expected for the driver to call memWriteReply on finish
    */
    void (*write)(const struct gnwDeviceMemWriteQuery * const);
    
    /*
        Decoder for gnwDeviceMemWriteQuery objects

        Required if write defined
    */
    gnwDeviceMemWriteQueryDecoder writeDecoder;
};

struct gnwDeviceUHA_mem {
    struct gnwDeviceUHA_mem_desc desc;
    struct gnwDeviceUHA_mem_routine routine;
};

#endif // GUNWOS_GUNWUHA_MEM_H