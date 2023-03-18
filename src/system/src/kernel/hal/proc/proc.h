//
//  proc.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#ifndef PROC_H
#define PROC_H

#include <types.h>
#include <hal/cpu/cpu.h>
#include <hal/gdt/gdt.h>

#define MAX_PROC 16
#define NONE_PROC_ID -2
#define KERNEL_PROC_ID -1

struct k_proc_descriptor {
    ptr_t img;
    addr_t entry;
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

    KERNEL_PROC_ID for kernel
*/
procId_t k_proc_getCurrentId();

/*
    Returns information about the process with given procId
*/
struct k_proc_process k_proc_getInfo(const procId_t procId);

/*
    Spawning new userland processes
*/
enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const);

/*
    Switching from kernel to process

    Params:
    * procId - Identifier of the next process
*/
void k_proc_switch(const procId_t procId);

/*
    Switching from process to kernel (if needed)

    Params:
    * refEsp - Reference stack pointer
    * currentProcId - Identifier of the current process
*/
void k_proc_switchToKernelIfNeeded(const uint_32 refEsp, const procId_t currentProcId);

/*
    Invoking callback functions inside process with given ID

    Params:
    * procId - identifier of the process funPtr() is going to be executed in
    * funPtr - function pointer relative to procId process memory
    * p* - parameters of various sizes
*/
void k_proc_callback_invoke_32(const procId_t procId, void (*funPtr)(uint_32), uint_32 p1);
void k_proc_callback_invoke_32_8(const procId_t procId, void (*funPtr)(uint_32, uint_8), uint_32 p1, uint_8 p2);

#endif // PROC_H
