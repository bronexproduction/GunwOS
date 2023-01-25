//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.12.2022.
//

#ifndef MEM_H
#define MEM_H

#include <stdgunw/types.h>

/*
    Returns system memory size in bytes
*/
size_t s_mem_totalMemBytes();

/*
    Returns free system memory size in bytes
*/
size_t s_mem_freeMemBytes();

#endif // MEM_H
