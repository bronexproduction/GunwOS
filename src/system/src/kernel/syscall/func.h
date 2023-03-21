//
//  func.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef FUNC_H
#define FUNC_H

#include <utils.h>
#include <types.h>

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; __asm__ volatile ("ret"); }

/*
    Returns valid absolute address of relPtr for given process
    or nullptr if process is invalid 
    or in case of access violation

    Params:
    * procId - process identifler
    * relPtr - pointer relative to process memory
    * refSize - size of buffer/datatype referenced by the relPtr in bytes
*/
ptr_t k_scl_func_getValidAbsoluteForProc(const procId_t procId, const ptr_t relPtr, const size_t refSize);

/*
    Returns valid absolute address of relPtr for current process
    or nullptr if process is invalid 
    or in case of access violation

    Params:
    * relPtr - pointer relative to process memory
    * refSize - size of buffer/datatype referenced by the relPtr in bytes
*/
ptr_t k_scl_func_getValidAbsoluteForCurrentProc(const ptr_t relPtr, const size_t refSize);

#endif // FUNC_H
