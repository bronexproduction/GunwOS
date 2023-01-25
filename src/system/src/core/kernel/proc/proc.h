//
//  proc.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef PROC_H
#define PROC_H

#include <stdgunw/types.h>
#include "../hal/cpu/cpu.h"

enum k_proc_state {
    RUNNING,
    READY
};

struct k_proc_process {
    /*
        Process identifier (pTab index + 1)
    */
    size_t identifier;

    /*
        Process state
    */
    enum k_proc_state state;

    /*
        Process CPU state
    */
    struct k_cpu_state cpuState;
};

#define MAX_PROC 16

extern struct k_proc_process pTab[MAX_PROC];

#endif // PROC_H
