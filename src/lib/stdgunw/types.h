//
//  types.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.01.2018.
//

// 32-bit only

#ifndef STDGUNW_TYPES_H
#define STDGUNW_TYPES_H

typedef signed char int_8;
typedef unsigned char uint_8;
typedef signed short int_16;
typedef unsigned short uint_16;
typedef signed long int_32;
typedef unsigned long uint_32;
typedef signed long long int_64;
typedef unsigned long long uint_64;

typedef uint_8* ptr_t;
typedef uint_32 size_t;
// TODO move to 64-bit time
typedef uint_32 time_t;

typedef struct {
    int x, y;
} point_t;

#ifndef __cplusplus
    typedef uint_8 bool;
#endif

#endif // STDGUNW_TYPES_H
