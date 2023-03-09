//
//  gunwuha_mem_desc.h
//  Universal Hardware Abstraction for memory-mapped devices
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_MEM_DESC_H
#define GUNWOS_GUNWUHA_MEM_DESC_H

#include <stdgunw/types.h>

struct gnwDeviceUHA_mem_desc {
    /*
        Size of mapped memory in bytes
    */
    size_t sizeBytes;
    
    /*
        Size of accepted write buffer
    */
    size_t inputSizeBytes;
};

#endif // GUNWOS_GUNWUHA_MEM_DESC_H
