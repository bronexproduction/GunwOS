//
//  func.h
//  GunwOS
//
//  Created by Artur Danielewski on 03.03.2023.
//

#ifndef FUNC_H
#define FUNC_H

#include "params.h"

#include <hal/cpu/cpu.h>

/*
    Syscall service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/
#define SCR(NAME, CODE) __attribute__((naked)) void k_scr_ ## NAME () { CODE; CPU_RETURN; }

#endif // FUNC_H
