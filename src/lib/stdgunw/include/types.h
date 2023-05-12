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

_Static_assert(sizeof(int_8) == 1, "Unexpected int_8 size");
_Static_assert(sizeof(uint_8) == 1, "Unexpected uint_8 size");
_Static_assert(sizeof(int_16) == 2, "Unexpected int_16 size");
_Static_assert(sizeof(uint_16) == 2, "Unexpected uint_16 size");
_Static_assert(sizeof(int_32) == 4, "Unexpected int_32 size");
_Static_assert(sizeof(uint_32) == 4, "Unexpected uint_32 size");
_Static_assert(sizeof(int_64) == 8, "Unexpected int_64 size");
_Static_assert(sizeof(uint_64) == 8, "Unexpected uint_64 size");

typedef uint_32 addr_t;
typedef uint_8* ptr_t;
typedef const uint_8* cptr_t;
typedef uint_32 size_t;
typedef uint_64 hugeSize_t;
// TODO move to 64-bit time
typedef uint_32 time_t;
typedef int_32 procId_t;
_Static_assert(sizeof(addr_t) == sizeof(ptr_t), "sizeof(addr_t) != sizeof(ptr_t)");
_Static_assert(sizeof(ptr_t) == sizeof(size_t), "sizeof(ptr_t) != sizeof(size_t)");
_Static_assert(sizeof(size_t) == sizeof(uint_32), "sizeof(size_t) != sizeof(uint_32)");

typedef struct {
    int x, y;
} point_t;

typedef struct {
    size_t offset;
    size_t length;
} range_size_t;

typedef struct {
    addr_t offset;
    size_t sizeBytes;
} range_addr_t;

#ifndef __cplusplus
    #define true  1
    #define false 0
    typedef uint_8 bool;
#endif

#endif // STDGUNW_TYPES_H
