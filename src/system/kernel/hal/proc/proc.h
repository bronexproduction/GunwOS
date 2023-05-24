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
#include <src/_gunwrlp.h>

#define MAX_PROC 4
#define NONE_PROC_ID -2
#define KERNEL_PROC_ID -1

struct k_proc_descriptor {
    ptr_t img;
    addr_t entry;
    size_t imgBytes;
};

enum k_proc_error {
    PE_NONE = 0,
    PE_LIMIT_REACHED,
    PE_ACCESS_VIOLATION,
    PE_OPERATION_FAILED,
    PE_INVALID_PARAMETER,
    PE_IGNORED,
    PE_UNKNOWN
};

enum k_proc_state {
    PS_NONE = 0,
    PS_NEW,
    PS_READY,
    PS_RUNNING,
    PS_BLOCKED,
    PS_FINISHED
};

enum k_proc_lockType {
    PLT_EVENT   = 1 << 0,
    PLT_IPC     = 1 << 1
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
    Blocking the process
    e.g. in case the process waits for an event

    Params:
    * procId - Identifier of the process
    * lockType - type/source of the lock
*/
void k_proc_lock(const procId_t procId, const enum k_proc_lockType lockType);

/*
    Removing a lock from the process and resuming alive process if able

    Params:
    * procId - identifier of the process to be resumed
    * lockType - type of lock to be released
*/
void k_proc_unlock(const procId_t procId, const enum k_proc_lockType lockType);

/*
    Stopping and cleaning up after running process

    Params:
    * procId - Identifier of the process
*/
void k_proc_stop(const procId_t procId);

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
    * pSizeBytes - buffer size in bytes (in case p is a pointer)
    * pDecodedSizeBytes - size in bytes of the decoded object 
                          (can be smaller than pSizeBytes, as decoded object fields may point directly to buffer offsets)
    * encoder - function converting object pointed by 'p' to a bytes array of 'pSizeBytes' bytes
    * decoder - function converting array of bytes of 'pSizeBytes' bytes to an object (reverse encoder)
    
    Note: encoder/decoder has to align object pointers after copying data to the new location
    
    Return value: enum k_proc_error - PE_NONE on success
*/
enum k_proc_error k_proc_callback_invoke_ptr(const procId_t procId,
                                             void (* const funPtr)(ptr_t),
                                             const ptr_t p,
                                             const size_t pSizeBytes,
                                             const size_t pDecodedSizeBytes,
                                             const gnwRunLoopDataEncodingRoutine encoder,
                                             const gnwRunLoopDataEncodingRoutine decoder);

#endif // PROC_H
