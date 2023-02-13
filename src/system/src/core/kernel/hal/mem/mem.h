//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.02.2023.
//

#ifndef MEM_H
#define MEM_H

#define KB(C) (C * 1024)
#define MB(C) (KB(C) * 1024)
#define GB(C) (MB(C) * 1024)

#define MEM_XMS_START               MB(1)
#define MEM_KERNEL_START            MEM_XMS_START
#define MEM_KERNEL_RESERVED_BYTES   MB(2)
#define MEM_MMIO_RESERVED_BYTES     MB(256)

#endif // MEM_H
