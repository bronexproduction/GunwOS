//
//  utils.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2018.
//

#ifndef UTILS_H
#define UTILS_H

#include "types.h"

#define STR_(X) #X
#define STR(X) STR_(X)

#define MAX(A,B) (A > B ? A : B)
#define MIN(A,B) (A < B ? A : B)
#define IN_RANGE(B, V, T) (B <= V && V <= T)

#define REG(BITS, NAME, REG_NAME) register uint_ ## BITS NAME __asm__ ( STR(REG_NAME) ); (void)(NAME);

size_t aligned(const size_t value, const size_t alignment);

#endif // UTILS_H
