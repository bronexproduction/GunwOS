//
//  mem.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "types.h"

void memset(void *ptr, const uint_8 val, size_t count);
void memzero(void *ptr, size_t count);
void memcopy(const void * const src, void *dst, size_t count);
