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
#include "../gdt/gdt.h"

#define MAX_PROC 16

struct k_proc_descriptor {
    ptr_t img;
    size_t imgBytes;
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
        Process privilege level
    */
    enum k_gdt_dpl dpl;
};

/*
    Returns current process identifier

    -1 for kernel
*/
int_32 k_proc_getCurrentId();

/*
    Returns information about the process with given procId
*/
struct k_proc_process k_proc_getInfo(const int_32 procId);

/*
    Spawning new userland processes
*/
enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const);

/*
    Switching from kernel to process

    Params:
    * procId - Identifier of the next process
*/
void k_proc_switch(const int_32 procId);

/*
    Switching from process to kernel (if needed)

    Params:
    * refEsp - Reference stack pointer
    * currentProcId - Identifier of the current process
*/
void k_proc_switchToKernelIfNeeded(const uint_32 refEsp, const int_32 currentProcId);

#endif // PROC_H
