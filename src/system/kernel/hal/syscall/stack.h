//
//  stack.h
//  GunwOS
//
//  Created by Artur Danielewski on 11.02.2024.
//

#ifndef STACK_H
#define STACK_H

register ptr_t kernelStack __asm__ ("esp");

#define _STACK_REFERENCE_OFFSET_ENTRY 0x2C
#define _STACK_REFERENCE_OFFSET_FUNC 0x30

#define _USER_STACK_PTR_ENTRY (ptr_t)(*((addr_t *)(kernelStack + _STACK_REFERENCE_OFFSET_ENTRY + 12)))
#define _USER_STACK_PTR_FUNC (ptr_t)(*((addr_t *)(kernelStack + _STACK_REFERENCE_OFFSET_FUNC + 12)))

#define FUNC_PTR (uint_32 *)(_USER_STACK_PTR_ENTRY + 0)

#define _PAR_PTR(NAME, OFFSET) addr_t * NAME = (addr_t *)(_USER_STACK_PTR_FUNC + OFFSET);
#define PAR_PTR_1(NAME) _PAR_PTR(NAME, 4)
#define PAR_PTR_2(NAME) _PAR_PTR(NAME, 8)
#define PAR_PTR_3(NAME) _PAR_PTR(NAME, 12)


#endif // STACK_H
