//
//  memmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.12.2022.
//

#include <stdgunw/types.h>

/*
    Returns system memory size in bytes
*/
size_t s_memmgr_totalMemBytes();

/*
    Returns free system memory size in bytes
*/
size_t s_memmgr_freeMemBytes();
