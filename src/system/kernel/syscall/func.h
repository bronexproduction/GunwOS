//
//  func.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef FUNC_H
#define FUNC_H

#include <types.h>
#include <error/panic.h>

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; __asm__ volatile ("ret"); }

#define SCLF_GET_VALID_ABS_NAMED(TYPE, NAME, VALUE, SIZEBYTES, ERR_HANDLE) \
    TYPE abs_ ## NAME = (TYPE)k_scl_func_getValidAbsoluteForProc(procId, (const ptr_t)(VALUE), SIZEBYTES); \
    if (!abs_ ## NAME) { OOPS("Invalid pointer referenced"); {ERR_HANDLE;} }
#define SCLF_GET_VALID_ABS(TYPE, VALUE, SIZEBYTES, ERR_HANDLE) SCLF_GET_VALID_ABS_NAMED(TYPE, VALUE, VALUE, SIZEBYTES, ERR_HANDLE)

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

#endif // FUNC_H
