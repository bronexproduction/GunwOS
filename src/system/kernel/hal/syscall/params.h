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

#define _USER_STACK_PTR (ptr_t)(*((addr_t *)(refEsp + _STACK_REFERENCE_OFFSET + 12)))

#define FUNC_CODE_PTR (addr_t *)(_USER_STACK_PTR + 0)

#define _PAR_PTR(NAME, OFFSET) addr_t * NAME = (addr_t *)(_USER_STACK_PTR + OFFSET);
#define PAR_PTR_1(NAME) _PAR_PTR(NAME, 4)
#define PAR_PTR_2(NAME) _PAR_PTR(NAME, 8)
#define PAR_PTR_3(NAME) _PAR_PTR(NAME, 12)

#define RESULT_PTR (addr_t *)(_USER_STACK_PTR + 0)

#endif // PARAMS_H
