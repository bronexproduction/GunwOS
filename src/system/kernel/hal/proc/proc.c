//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <mem.h>
#include <defs.h>
#include <string.h>

#include <schedule/schedule.h>
#include <hal/criticalsec/criticalsec.h>
#include <hal/mem/mem.h>
#include <ipc/ipc.h>
#include <timer/timer.h>
#include <error/panic.h>
#include <queue/queue.h>
#include <runloop/runloop.h>

#define STACK_VAL(REFESP, SIZE, OFFSET) (*(uint_ ## SIZE *)(REFESP + OFFSET))

PRIVATE struct process_kernel {
    /*
        Process info
    */
    struct k_proc_process info;

    /*
        Process CPU state
    */
    struct k_cpu_state cpuState;
} kernelProc;

PRIVATE struct process_user {
    /*
        Process info
    */
    struct k_proc_process info;

    /*
        Process CPU state
    */
    struct k_cpu_state cpuState;

    /*
        Process memory paging information
    */
    struct k_virtual_paging_info_t pagingInfo;

    /*
        Lock mask (if BLOCKED)
    */
    enum k_proc_lockType lockMask;
} pTab[MAX_PROC];

static procId_t procCurrent = KERNEL_PROC_ID;

procId_t k_proc_getCurrentId() {
    return procCurrent;
}

bool k_proc_idIsUser(const procId_t procId) {
    return procId >= 0;
}

struct k_proc_process k_proc_getInfo(const procId_t procId) {
    if (procId < KERNEL_PROC_ID || procId >= MAX_PROC) {
        struct k_proc_process info;
        memzero(&info, sizeof(struct k_proc_process));
        OOPS("Process id out of range", info);
    }
    
    return (procId == KERNEL_PROC_ID) ? kernelProc.info : pTab[procId].info;
}

enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const descriptor) {
    uint_32 pIndex;
    
    CRITICAL_SECTION_BEGIN {
        for (pIndex = 0; pIndex < MAX_PROC; ++pIndex) {
            if (pTab[pIndex].info.state == PS_NONE) {
                break;
            }
        }

        if (pIndex >= MAX_PROC) {
            CRITICAL_SECTION_END;
            return PE_LIMIT_REACHED;
        }

        pTab[pIndex].info.state = PS_NEW;
        
    } CRITICAL_SECTION_END;

    memzero(&pTab[pIndex].cpuState, sizeof pTab[pIndex].cpuState);

    pTab[pIndex].info.dpl = DPL_3;

    const size_t stackBytes = KiB(512);
    ptr_t codeStartPtr = descriptor->img;
    ptr_t codeEndPtr = codeStartPtr + descriptor->imgBytes - 1;
    ptr_t dataStartPtr = codeEndPtr + 1;
    ptr_t dataEndPtr = dataStartPtr + stackBytes - 1;

    if (codeStartPtr < GDT_SEGMENT_START(r3code)) {
        OOPS("Attempted to spawn process below its code segment", PE_UNKNOWN);
    }
    if (codeEndPtr > GDT_SEGMENT_END(r3code)) {
        OOPS("Attempted to spawn process above its code segment", PE_UNKNOWN);
    }
    if (codeEndPtr <= codeStartPtr) {
        OOPS("Attempted to spawn process outside its code segment", PE_UNKNOWN);
    }
    if (dataStartPtr < GDT_SEGMENT_START(r3data)) {
        OOPS("Attempted to spawn process below its data segment", PE_UNKNOWN);
    }
    if (dataEndPtr > GDT_SEGMENT_END(r3data)) {
        OOPS("Attempted to spawn process above its data segment", PE_UNKNOWN);
    }
    if (dataEndPtr <= dataStartPtr) {
        OOPS("Attempted to spawn process outside its data segment", PE_UNKNOWN);
    }
    if (dataEndPtr <= codeStartPtr) {
        OOPS("Attempted to spawn process outside its segments", PE_UNKNOWN);
    }

    addr_t relativeImgPtr = codeStartPtr - GDT_SEGMENT_START(r3code);
    addr_t relativeEntryPtr = relativeImgPtr + descriptor->entry;
    addr_t relativeStackPtr = dataEndPtr - GDT_SEGMENT_START(r3data);

    pTab[pIndex].cpuState.esp = (uint_32)relativeStackPtr;
    pTab[pIndex].cpuState.eip = (uint_32)relativeEntryPtr;
    pTab[pIndex].cpuState.eflags = FLAGS_INTERRUPT;

    pTab[pIndex].cpuState.cs = (uint_16)(GDT_OFFSET(r3code) | pTab[pIndex].info.dpl);
    pTab[pIndex].cpuState.ds = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].info.dpl);
    pTab[pIndex].cpuState.es = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].info.dpl);
    pTab[pIndex].cpuState.fs = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].info.dpl);
    pTab[pIndex].cpuState.gs = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].info.dpl);
    pTab[pIndex].cpuState.ss = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].info.dpl);

    pTab[pIndex].info.state = PS_READY;
    k_proc_schedule_didSpawn(pIndex);

    return PE_NONE;
}

void k_proc_lock(const procId_t procId, enum k_proc_lockType lockType) {
    if (procId <= KERNEL_PROC_ID || procId >= MAX_PROC) {
        OOPS("Process id out of range",);
    }

    pTab[procId].lockMask |= lockType;
    
    if (pTab[procId].info.state == PS_BLOCKED) {
        return;
    }
    if (pTab[procId].info.state != PS_READY &&
        pTab[procId].info.state != PS_RUNNING) {
        OOPS("Unexpected process state during lock",);
    }

    pTab[procId].info.state = PS_BLOCKED;
    k_proc_schedule_processStateDidChange();
}

void k_proc_unlock(const procId_t procId, enum k_proc_lockType lockType) {
    if (procId <= KERNEL_PROC_ID || procId >= MAX_PROC) {
        OOPS("Process id out of range",);
    }

    pTab[procId].lockMask &= ~lockType;

    if (pTab[procId].info.state != PS_BLOCKED ||
        pTab[procId].lockMask) {
        return;
    }
    
    pTab[procId].info.state = PS_READY;
    k_proc_schedule_processStateDidChange();
}

static bool isProcessAlive(const procId_t procId) {
    return pTab[procId].info.state == PS_READY ||
           pTab[procId].info.state == PS_RUNNING ||
           pTab[procId].info.state == PS_BLOCKED;
}

static void procCleanup(const procId_t procId) {
    if (procId <= KERNEL_PROC_ID || procId >= MAX_PROC) {
        OOPS("Process id out of range",);
    }
    if (isProcessAlive(procId)) {
        OOPS("Unexpected process state during cleanup",);
    }

    memzero(&pTab[procId], sizeof(struct process_user));
    k_mem_procCleanup(procId);
}

void k_proc_stop(const procId_t procId) {
    if (procId <= KERNEL_PROC_ID || procId >= MAX_PROC) {
        OOPS("Process id out of range",);
    }
    if (!isProcessAlive(procId)) {
        OOPS("Unexpected process state during stop",);
    }
    
    pTab[procId].info.state = PS_FINISHED;
    k_que_dispatch_arch((fPtr_arch)procCleanup, procId);
    k_proc_schedule_processStateDidChange();
}

/*
    Switching between processes

    k_proc_switch
    k_proc_switchKernel

    For more information
    refer to Intel i386 Programmer's Reference Manual (1986)
    section 7.5 Task Switching
    and 
    section 9.6.1 Interrupt Procedures
*/
void k_proc_switch(const procId_t procId) {
    if (procId < 0) {
        OOPS("Invalid next process id during switch",);
    }
    if (procId >= MAX_PROC) {
        OOPS("Next process id over limit during switch",);
    }
    if (kernelProc.info.state != PS_RUNNING) {
        OOPS("Invalid kernel process state during switch",);
    }
    if (pTab[procId].info.state != PS_READY) {
        OOPS("Invalid next process state during switch",);
    }
    if (pTab[procId].lockMask) {
        OOPS("Attempted switch to locked process",);
    }
    
    kernelProc.info.state = PS_READY;
    pTab[procId].info.state = PS_RUNNING;

    procCurrent = procId;

    // Save kernel CPU state
    #warning probably a lot of these operations can be removed
    CPU_PUSH
    {
        kernelProc.cpuState.edi = STACK_VAL(k_cpu_stackPtr, 32, 0);
        kernelProc.cpuState.esi = STACK_VAL(k_cpu_stackPtr, 32, 4);
        kernelProc.cpuState.ebp = STACK_VAL(k_cpu_stackPtr, 32, 8);
        kernelProc.cpuState.ebx = STACK_VAL(k_cpu_stackPtr, 32, 16);
        kernelProc.cpuState.edx = STACK_VAL(k_cpu_stackPtr, 32, 20);
        kernelProc.cpuState.ecx = STACK_VAL(k_cpu_stackPtr, 32, 24);
        kernelProc.cpuState.eax = STACK_VAL(k_cpu_stackPtr, 32, 28);
        kernelProc.cpuState.gs  = STACK_VAL(k_cpu_stackPtr, 16, 32);
        kernelProc.cpuState.fs  = STACK_VAL(k_cpu_stackPtr, 16, 34);
        kernelProc.cpuState.es  = STACK_VAL(k_cpu_stackPtr, 16, 36);
        kernelProc.cpuState.ds  = STACK_VAL(k_cpu_stackPtr, 16, 38);        
    }
    CPU_POP

    extern ptr_t k_proc_kernelRet;
    kernelProc.cpuState.eip = (uint_32)&k_proc_kernelRet;
    kernelProc.cpuState.esp = (uint_32)k_cpu_stackPtr;
    __asm__ volatile ("pushw %cs"); __asm__ volatile ("popw %[mem]" : [mem] "=m" (kernelProc.cpuState.cs));
    __asm__ volatile ("pushfl");    __asm__ volatile ("popl %[mem]" : [mem] "=m" (kernelProc.cpuState.eflags));
    __asm__ volatile ("pushw %ss"); __asm__ volatile ("popw %[mem]" : [mem] "=m" (kernelProc.cpuState.ss));

    // Restore next process CPU state

    const struct k_cpu_state * const nextCpuState = &pTab[procId].cpuState;

    // Set kernel stack pointer in TSS
    k_cpu_tss.esp0 = (uint_32)k_cpu_stackPtr;

    // Prepare IRET stack
    __asm__ volatile ("pushw $0");
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->ss));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->esp));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->eflags));
    __asm__ volatile ("pushw $0");
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->cs));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->eip));
    
    // Restore general purpose registers
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->ds));
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->es));
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->fs));
    __asm__ volatile ("pushw %[mem]" : : [mem] "m" (nextCpuState->gs));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->eax));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->ecx));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->edx));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->ebx));
    __asm__ volatile ("pushl %esp");
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->ebp));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->esi));
    __asm__ volatile ("pushl %[mem]" : : [mem] "m" (nextCpuState->edi));
    CPU_POP

    // Call iret to move to the next process
    CPU_INTERRUPT_RETURN;

    // Kernel return location
    __asm__ volatile ("k_proc_kernelRet:");
}

void k_proc_switchToKernelIfNeeded(const uint_32 refEsp, const procId_t currentProcId) {
    if (kernelProc.info.state == PS_RUNNING) {
        return;
    }
    if (currentProcId < 0) {
        OOPS("Invalid current process id during switch",);
    }
    if (currentProcId >= MAX_PROC) {
        OOPS("Current process id over limit during switch",);
    }
    if (pTab[currentProcId].info.state != PS_RUNNING &&
        pTab[currentProcId].info.state != PS_BLOCKED &&
        pTab[currentProcId].info.state != PS_FINISHED) {
        OOPS("Invalid current process state during switch",);
    }
    if (kernelProc.info.state != PS_READY) {
        OOPS("Invalid kernel process state during switch",);
    }
    if (!refEsp) {
        OOPS("Invalid reference stack pointer during switch",);
    }
    
    kernelProc.info.state = PS_RUNNING;
    procCurrent = KERNEL_PROC_ID;

    if (pTab[currentProcId].info.state != PS_FINISHED) {
        if (pTab[currentProcId].info.state == PS_RUNNING) {
            pTab[currentProcId].info.state = PS_READY;
        }
        struct k_cpu_state * const currentCpuState = &pTab[currentProcId].cpuState;
        
        // D       31              0         
        // I       ╔═══════╦═══════╗◄──┐     
        // R       ║▒▒▒▒▒▒▒║OLD SS ║   │     
        // E       ╠═══════╩═══════╣ SS:ESP  
        // C       ║    OLD ESP    ║ FROM TSS
        // T       ╠═══════════════╣◄── refEsp + 52    
        // I       ║  OLD EFLAGS   ║         
        // O       ╠═══════╦═══════╣◄── refEsp + 48    
        // N       ║▒▒▒▒▒▒▒║OLD CS ║ 
        //         ╠═══════╩═══════╣◄── refEsp + 44 
        // O       ║    OLD EIP    ║        
        // F       ╠═══════╦═══════╣◄── refEsp + 40
        //         ║OLD DS ║OLD ES ║
        // E       ╠═══════╬═══════╣◄── refEsp + 36
        // X       ║OLD FS ║OLD GS ║
        // P       ╠═══════╩═══════╣◄── refEsp + 32
        // A       ║    OLD EAX    ║
        // N       ╠═══════════════╣◄── refEsp + 28
        // S       ║    OLD ECX    ║
        // I       ╠═══════════════╣◄── refEsp + 24
        // O       ║    OLD EDX    ║
        // N       ╠═══════════════╣◄── refEsp + 20
        //         ║    OLD EBX    ║
        // │       ╠═══════════════╣◄── refEsp + 16
        // │       ║ ESP (IGNORED) ║
        // ▼       ╠═══════════════╣◄── refEsp + 12
        //         ║    OLD EBP    ║
        //         ╠═══════════════╣◄── refEsp + 8
        //         ║    OLD ESI    ║ 
        //         ╠═══════════════╣◄── refEsp + 4   
        //         ║    OLD EDI    ║   
        //         ╠═══════════════╣◄── refEsp
        //         ·               ·

        // Save current process CPU state

#warning Syscalls implementation shows that ESP of the calling process is on offset 12, not 52

        currentCpuState->edi    = STACK_VAL(refEsp, 32, 0);
        currentCpuState->esi    = STACK_VAL(refEsp, 32, 4);
        currentCpuState->ebp    = STACK_VAL(refEsp, 32, 8);
        currentCpuState->ebx    = STACK_VAL(refEsp, 32, 16);
        currentCpuState->edx    = STACK_VAL(refEsp, 32, 20);
        currentCpuState->ecx    = STACK_VAL(refEsp, 32, 24);
        currentCpuState->eax    = STACK_VAL(refEsp, 32, 28);
        currentCpuState->gs     = STACK_VAL(refEsp, 16, 32);
        currentCpuState->fs     = STACK_VAL(refEsp, 16, 34);
        currentCpuState->es     = STACK_VAL(refEsp, 16, 36);
        currentCpuState->ds     = STACK_VAL(refEsp, 16, 38);
        currentCpuState->eip    = STACK_VAL(refEsp, 32, 40);
        currentCpuState->cs     = STACK_VAL(refEsp, 16, 44);
        currentCpuState->eflags = STACK_VAL(refEsp, 32, 48);
        currentCpuState->esp    = STACK_VAL(refEsp, 32, 52);
        currentCpuState->ss     = STACK_VAL(refEsp, 16, 56);
    }

    // Restore kernel CPU state

    STACK_VAL(refEsp, 32, 0)  = kernelProc.cpuState.edi;
    STACK_VAL(refEsp, 32, 4)  = kernelProc.cpuState.esi;
    STACK_VAL(refEsp, 32, 8)  = kernelProc.cpuState.ebp;
    STACK_VAL(refEsp, 32, 16) = kernelProc.cpuState.ebx;
    STACK_VAL(refEsp, 32, 20) = kernelProc.cpuState.edx;
    STACK_VAL(refEsp, 32, 24) = kernelProc.cpuState.ecx;
    STACK_VAL(refEsp, 32, 28) = kernelProc.cpuState.eax;
    STACK_VAL(refEsp, 16, 32) = kernelProc.cpuState.gs;
    STACK_VAL(refEsp, 16, 34) = kernelProc.cpuState.fs;
    STACK_VAL(refEsp, 16, 36) = kernelProc.cpuState.es;
    STACK_VAL(refEsp, 16, 38) = kernelProc.cpuState.ds;
    STACK_VAL(refEsp, 32, 40) = kernelProc.cpuState.eip;
    STACK_VAL(refEsp, 16, 44) = kernelProc.cpuState.cs;
    STACK_VAL(refEsp, 32, 48) = kernelProc.cpuState.eflags;
    STACK_VAL(refEsp, 32, 52) = kernelProc.cpuState.esp;
    STACK_VAL(refEsp, 16, 56) = kernelProc.cpuState.ss;
}

#include <log/log.h>

static enum k_proc_error callbackInvoke(const procId_t procId,
                                        const enum gnwEventFormat format, 
                                        const ptr_t funPtr, 
                                        const ptr_t p,
                                        const size_t pSizeBytes,
                                        const size_t pEncodedSizeBytes,
                                        const gnwRunLoopDataEncodingRoutine encoder,
                                        const gnwRunLoopDataEncodingRoutine decoder) {

    {
        char msg[31] = "callbackInvoke - new dispatch ";
        LOG(msg);
    }
    {
        char msg[32] = "  target proc -         ";
        uint2dec((addr_t)procId, msg + 16);
        LOG(msg);
    }
    {
        char msg[27] = "  format -         ";
        uint2dec((addr_t)format, msg + 11);
        LOG(msg);
    }
    {
        char msg[128] = "  dispatched fun ptr -         ";
        uint2hex((addr_t)funPtr, msg + 23);
        LOG(msg);
    }
    {
        char msg[128] = "  parameter ptr -         ";
        uint2hex((addr_t)p, msg + 18);
        LOG(msg);
    }
    {
        char msg[35] = "  parameter size -         ";
        uint2dec((addr_t)pSizeBytes, msg + 19);
        LOG(msg);
    }
    {
        char msg[43] = "  encoded parameter size -         ";
        uint2dec((addr_t)pEncodedSizeBytes, msg + 27);
        LOG(msg);
    }
    {
        char msg[128] = "  encoder -         ";
        uint2hex((addr_t)encoder, msg + 12);
        LOG(msg);
    }
    {
        char msg[128] = "  decoder -         ";
        uint2hex((addr_t)decoder, msg + 12);
        LOG(msg);
    }
    {
        char msg[9] = "  bytes:";
        LOG_BLOCK(
            LOG_NBR(msg);
            for (size_t i = 0; i < pSizeBytes; ++i) {
                char byteString[3] = { 0 };
                uint2hex((addr_t)((uint_8 *)p)[i], byteString);
                LOG_NBR(" ");
                LOG_NBR(byteString);
            }
        );
    }

    if (procId <= KERNEL_PROC_ID || procId >= MAX_PROC) {
        OOPS("Process id out of range", PE_UNKNOWN);
    }
    if (pTab[procId].info.state == PS_FINISHED) {
        return PE_IGNORED;
    }
    if (!isProcessAlive(procId)) {
        OOPS("Attempted callback invocation in dead process", PE_UNKNOWN);
    }
    
    struct gnwRunLoopDispatchItem item;
    item.format = format;
    switch (format) {
    case GEF_VOID:
        item.routine._void = (gnwEventListener_void)funPtr;
        break;
    case GEF_PTR:
        item.routine._ptr = (gnwEventListener_ptr)funPtr;
        break;
    case GEF_NONE:
        OOPS("Unexpected event format", PE_UNKNOWN);
    }
    item.dataSizeBytes = pEncodedSizeBytes;
    item.decodedDataSizeBytes = pSizeBytes;
    item.decode = decoder;

    enum gnwRunLoopError err;
    size_t runloopToken;
    CRITICAL_SECTION(
        err = k_runloop_reserve(procId, &runloopToken);
    )
    if (err == GRLE_FULL) {
        return PE_LIMIT_REACHED;
    } else if (err != GRLE_NONE) {
        return PE_OPERATION_FAILED;
    }
    
    err = k_runloop_dispatch(procId, runloopToken, item, p, encoder);
    if (err != GRLE_NONE) {
        return PE_OPERATION_FAILED;
    }

    k_proc_unlock(procId, PLT_EVENT);
    k_proc_schedule_processStateDidChange();

    return PE_NONE;
}

enum k_proc_error k_proc_callback_invoke_ptr(const procId_t procId,
                                             void (* const funPtr)(ptr_t),
                                             const ptr_t p,
                                             const size_t pSizeBytes,
                                             const size_t pEncodedSizeBytes,
                                             const gnwRunLoopDataEncodingRoutine encoder,
                                             const gnwRunLoopDataEncodingRoutine decoder) {
    return callbackInvoke(procId, GEF_PTR, (ptr_t)funPtr, p, pSizeBytes, pEncodedSizeBytes, encoder, decoder);
}

static void k_proc_prepareKernelProc() {
    memset(&kernelProc, 0, sizeof(struct process_kernel));
    kernelProc.info.state = PS_RUNNING;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_onTick();
    k_tmr_regMsHandler(k_proc_schedule_onTick);
}
