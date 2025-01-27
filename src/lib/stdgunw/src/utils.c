//
//  utils.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.03.2023.
//

#include "../include/utils.h"

size_t aligned(const size_t value, const size_t alignment) {
    return alignedr(value, alignment, true);
}

size_t alignedr(const size_t value, const size_t alignment, const bool roundUp) {
    if (value % alignment) {
        return (value / alignment) * alignment + (roundUp ? alignment : 0);
    }
    else {
        return value;
    }
}
