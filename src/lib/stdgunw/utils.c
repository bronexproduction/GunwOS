//
//  utils.c
//  GunwOS
//
//  Created by Artur Danielewski on 02.03.2023.
//

#include "include/utils.h"

size_t aligned(const size_t value, const size_t alignment) {
    if (value % alignment) {
        return (value / alignment) * alignment + alignment;
    }
    else {
        return value;
    }
}
