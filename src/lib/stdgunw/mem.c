//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "include/mem.h"

void memset(void *ptr, const uint_8 val, size_t bytes) {
    uint_8 *p = (uint_8 *)ptr;
    while (bytes--) {
        *(p++) = val;
    }
}

void memnull(void *ptr, size_t bytes) {
    memset(ptr, 0, bytes);
}

void memcopy(const void * const src, void *dst, size_t count) {
    while (count--) {
        ((uint_8 *)dst)[count] = ((uint_8 *)src)[count];
    }
}
