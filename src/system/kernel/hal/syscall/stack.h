//
//  stack.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2024.
//

#ifndef STACK_H
#define STACK_H

#include <types.h>

#define _STACK_REFERENCE_OFFSET_ENTRY 0x2C
#define _STACK_REFERENCE_OFFSET_FUNC 0x30

#define _USER_STACK_PTR(LOC) (ptr_t)(*((addr_t *)(k_cpu_stackPtr + _STACK_REFERENCE_OFFSET_ ## LOC + 12)))

#define FUNC_CODE_PTR (addr_t *)(_USER_STACK_PTR(ENTRY) + 0)

#define _PAR_PTR(NAME, OFFSET) addr_t * NAME = (addr_t *)(_USER_STACK_PTR(FUNC) + OFFSET);
#define PAR_PTR_1(NAME) _PAR_PTR(NAME, 4)
#define PAR_PTR_2(NAME) _PAR_PTR(NAME, 8)
#define PAR_PTR_3(NAME) _PAR_PTR(NAME, 12)

#endif // STACK_H
