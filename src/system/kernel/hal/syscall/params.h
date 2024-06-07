//
//  params.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2024.
//

#ifndef PARAMS_H
#define PARAMS_H

#include <types.h>

#define _STACK_REFERENCE_OFFSET 0x28

#define _USER_STACK_ADDR (*(addr_t *)(refEsp + _STACK_REFERENCE_OFFSET + 12))

/*
    Important note:
    
    While accessing memory _USER_STACK_ADDR
    bear in mind that it may technically overlap with reserved space

    Check for potential access violation
    before dereferencing memory at this address
*/

#define _USER_STACK_PTR (ptr_t)_USER_STACK_ADDR

#define FUNC_CODE_STACK_OFFSET      0
#define RESULT_STACK_OFFSET         0
#define PARAMETER_1_STACK_OFFSET    4
#define PARAMETER_2_STACK_OFFSET    8
#define PARAMETER_3_STACK_OFFSET    12

void * userStackSafeValuePointer(ptr_t refEsp, addr_t offset);

#endif // PARAMS_H
