//
//  gunwuha_mem_desc.h
//  Universal Hardware Abstraction for memory-mapped devices
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_MEM_DESC_H
#define GUNWOS_GUNWUHA_MEM_DESC_H

#include <types.h>

struct gnwDeviceUHA_mem_desc {
    /*
        Range of mapped memory in bytes
    */
    range_size_t bytesRange;
    
    /*
        Maximum size of accepted write buffer
    */
    size_t maxInputSizeBytes;
};

#endif // GUNWOS_GUNWUHA_MEM_DESC_H
