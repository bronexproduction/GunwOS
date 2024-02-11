//
//  func.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef FUNC_H
#define FUNC_H

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/

#define _STACK_REFERENCE_OFFSET 0x30
#define _USER_STACK_ADDR_PTR ((addr_t *)(stack + _STACK_REFERENCE_OFFSET + 12))
#define _USER_STACK_PTR (ptr_t)(*_USER_STACK_ADDR_PTR)

#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { register ptr_t stack __asm__ ("esp"); (void)stack; CODE; __asm__ volatile ("ret"); }

#define _PAR_PTR(NAME, OFFSET) addr_t * NAME = (addr_t *)(_USER_STACK_PTR + OFFSET);
#define PAR_PTR_1(NAME) _PAR_PTR(NAME, 4)
#define PAR_PTR_2(NAME) _PAR_PTR(NAME, 8)
#define PAR_PTR_3(NAME) _PAR_PTR(NAME, 12)


#endif // FUNC_H
