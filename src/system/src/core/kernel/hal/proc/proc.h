//
//  proc.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef PROC_H
#define PROC_H

#include <stdgunw/types.h>
#include "../cpu/cpu.h"

struct k_proc_descriptor {
    ptr_t img;
    ptr_t stack;
};

enum k_proc_error {
    PE_NONE = 0,
    PE_LIMIT_REACHED
};

enum k_proc_state {
    PS_NONE = 0,
    PS_NEW,
    PS_READY,
    PS_RUNNING,
    PS_BLOCKED
};

struct k_proc_process {
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

/*
    Spawning new userland processes
*/
enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const);

/*
    Switching between processes
*/
enum k_proc_error k_proc_switch(const size_t currentProcId, const size_t procId);

#endif // PROC_H