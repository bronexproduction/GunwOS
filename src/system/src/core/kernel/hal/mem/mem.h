//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef MEM_H
#define MEM_H

#include <stdgunw/defs.h>

#define MEM_XMS_START               MB(1)
#define MEM_KERNEL_START            MEM_XMS_START
#define MEM_KERNEL_RESERVED_BYTES   MB(2)
#define MEM_MMIO_RESERVED_BYTES     MB(256)

#endif // MEM_H
