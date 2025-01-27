//
//  mem.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2020.
//

#include "../include/mem.h"

void memset(void *ptr, const uint_8 val, size_t bytes) {
    byte_t *p = (byte_t *)ptr;
    while (bytes--) {
        *(p++) = val;
    }
}

void memzero(void *ptr, size_t bytes) {
    memset(ptr, 0, bytes);
}

void memcopy(const void * const src, void *dst, size_t count) {
    while (count--) {
        ((byte_t *)dst)[count] = ((byte_t *)src)[count];
    }
}
