//
//  utils.h
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2018.
//

#ifndef STDGUNW_UTILS_H
#define STDGUNW_UTILS_H

#include "types.h"

#define STR_(X) #X
#define STR(X) STR_(X)

#define MAX(A,B) (A > B ? A : B)
#define MIN(A,B) (A < B ? A : B)
#define IN_RANGE(B, V, T) (B <= V && V <= T)

#define SET(BIT) (BIT)
#define CLEAR(BIT) ((BIT) & 0)

#define IS_SET(BIT, REG) ((BIT) & (REG))
#define IS_NOT_SET(BIT, REG) (!IS_SET((BIT), (REG)))

#define BIT_RANGE_ALIGNED(VALUE, RANGE) (((VALUE) << __builtin_ctz(RANGE)) & (RANGE))

size_t aligned(const size_t value, const size_t alignment);
size_t alignedr(const size_t value, const size_t alignment, const bool roundUp);

#endif // STDGUNW_UTILS_H
