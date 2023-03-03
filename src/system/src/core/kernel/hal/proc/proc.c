//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <stdgunw/mem.h>
#include <stdgunw/defs.h>

#include "../../schedule/schedule.h"
#include "../criticalsec/criticalsec.h"
#include "../../timer/timer.h"
#include "../../error/panic.h"
#include "../gdt/gdt.h"
#include "../cpu/cpu.h"

register const uint_32 cur_esp __asm__ ("esp");
#define STACK_VAL(SIZE, OFFSET) (*(uint_ ## SIZE *)(cur_esp + OFFSET))

struct k_proc_process pTab[MAX_PROC];

enum k_proc_error k_proc_spawn(const struct k_proc_descriptor * const descriptor) {
    int pIndex;
    
    CRITICAL_SECTION_BEGIN {
        for (pIndex = 1; pIndex < MAX_PROC; ++pIndex) {
            if (pTab[pIndex].state == PS_NONE) {
                break;
            }
        }

        if (pIndex >= MAX_PROC) {
            CRITICAL_SECTION_END;
            return PE_LIMIT_REACHED;
        }

        pTab[pIndex].state = PS_NEW;
        
    } CRITICAL_SECTION_END;

    memnull(&pTab[pIndex].cpuState, sizeof pTab[pIndex].cpuState);

    pTab[pIndex].dpl = DPL_3;

    size_t stackBytes = MB(1);
    ptr_t codeStartPtr = descriptor->img;
    ptr_t codeEndPtr = codeStartPtr + descriptor->imgBytes - 1;
    ptr_t dataStartPtr = codeEndPtr + 1;
    ptr_t dataEndPtr = dataStartPtr + stackBytes - 1;

    if (codeStartPtr < GDT_SEGMENT_START(r3code)) {
        OOPS("Attempted to spawn process below its code segment")
    }
    if (codeEndPtr > GDT_SEGMENT_END(r3code)) {
        OOPS("Attempted to spawn process above its code segment")
    }
    if (codeEndPtr <= codeStartPtr) {
        OOPS("Attempted to spawn process outside its code segment");
    }
    if (dataStartPtr < GDT_SEGMENT_START(r3data)) {
        OOPS("Attempted to spawn process below its data segment")
    }
    if (dataEndPtr > GDT_SEGMENT_END(r3data)) {
        OOPS("Attempted to spawn process above its data segment")
    }
    if (dataEndPtr <= dataStartPtr) {
        OOPS("Attempted to spawn process outside its data segment");
    }
    if (dataEndPtr <= codeStartPtr) {
        OOPS("Attempted to spawn process outside its segments");
    }

    ptr_t relativeImgPtr = (ptr_t)(codeStartPtr - GDT_SEGMENT_START(r3code));
    ptr_t relativeStackPtr = (ptr_t)(dataEndPtr - GDT_SEGMENT_START(r3data));

    pTab[pIndex].cpuState.esp = (uint_32)relativeStackPtr;
    pTab[pIndex].cpuState.eip = (uint_32)relativeImgPtr;
    pTab[pIndex].cpuState.eflags = FLAGS_INTERRUPT;

    pTab[pIndex].cpuState.cs = (uint_16)(GDT_OFFSET(r3code) | pTab[pIndex].dpl);
    pTab[pIndex].cpuState.ds = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].dpl);
    pTab[pIndex].cpuState.es = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].dpl);
    pTab[pIndex].cpuState.fs = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].dpl);
    pTab[pIndex].cpuState.gs = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].dpl);
    pTab[pIndex].cpuState.ss = (uint_16)(GDT_OFFSET(r3data) | pTab[pIndex].dpl);

    k_proc_schedule_didSpawn(pIndex);

    return PE_NONE;
}

/*
    Switching between processes

    For more information
    refer to Intel i386 Programmer's Reference Manual (1986)
    section 7.5 Task Switching
    and 
    section 9.6.1 Interrupt Procedures
*/
void k_proc_switch(const uint_32 refEsp, const size_t currentProcId, const size_t nextProcId) {
    if (currentProcId == nextProcId)  {
        OOPS("Process identifers equal during switch");
    }
    if (currentProcId && nextProcId) {
        OOPS("Invalid process identifers during switch");
    }
    if (pTab[currentProcId].state != PS_RUNNING) {
        OOPS("Invalid current process state during switch");
    }
    if (pTab[nextProcId].state != PS_READY) {
        OOPS("Invalid next process state during switch");
    }
    if (!nextProcId && !refEsp) {
        OOPS("Invalid reference stack pointer during switch");
    }
    
    pTab[currentProcId].state = PS_READY;
    pTab[nextProcId].state = PS_RUNNING;

    // 7.5 Task Switching
    // The 80386 switches execution to another task in any of four cases:
    // 1. The current task executes a JMP or CALL that refers to a TSS descriptor.
    // 2. The current task executes a JMP or CALL that refers to a task gate.
    // 3. An interrupt or exception vectors to a task gate in the IDT.
    // 4. The current task executes an IRET when the NT flag is set.

    // Whether invoked as a task or as a procedure of the interrupted task, an
    // interrupt handler always returns control to the interrupted procedure in the
    // interrupted task. If the NT flag is set, however, the handler is an
    // interrupt task, and the IRET switches back to the interrupted task.
    // A task switching operation involves these steps:
    // 1. Checking that the current task is allowed to switch to the designated
    // task. Data-access privilege rules apply in the case of JMP or CALL
    // instructions. The DPL of the TSS descriptor or task gate must be less
    // than or equal to the maximum of CPL and the RPL of the gate selector.
    // Exceptions, interrupts, and IRETs are permitted to switch tasks
    // regardless of the DPL of the target task gate or TSS descriptor.
    // 2. Checking that the TSS descriptor of the new task is marked present
    // and has a valid limit. Any errors up to this point occur in the
    // context of the outgoing task. Errors are restartable and can be
    // handled in a way that is transparent to applications procedures.
    // 3. Saving the state of the current task. The processor finds the base
    // address of the current TSS cached in the task register. It copies the
    // registers into the current TSS (EAX, ECX, EDX, EBX, ESP, EBP, ESI,
    // EDI, ES, CS, SS, DS, FS, GS, and the flag register). The EIP field of
    // the TSS points to the instruction after the one that caused the task
    // switch.
    // 4. Loading the task register with the selector of the incoming task's
    // TSS descriptor, marking the incoming task's TSS descriptor as busy,
    // and setting the TS (task switched) bit of the MSW. The selector is
    // either the operand of a control transfer instruction or is taken from
    // a task gate.
    // 5. Loading the incoming task's state from its TSS and resuming
    // execution. The registers loaded are the LDT register; the flag
    // register; the general registers EIP, EAX, ECX, EDX, EBX, ESP, EBP,
    // ESI, EDI; the segment registers ES, CS, SS, DS, FS, and GS; and PDBR.
    // Any errors detected in this step occur in the context of the incoming
    // task. To an exception handler, it appears that the first instruction
    // of the new task has not yet executed.

    // 9.6.1.2 Returning from an Interrupt Procedure
    // An interrupt procedure also differs from a normal procedure in the method
    // of leaving the procedure. The IRET instruction is used to exit from an
    // interrupt procedure. IRET is similar to RET except that IRET increments EIP
    // by an extra four bytes (because of the flags on the stack) and moves the
    // saved flags into the EFLAGS register. The IOPL field of EFLAGS is changed
    // only if the CPL is zero. The IF flag is changed only if CPL ≤ IOPL.
    
    // Figure 9-5. Stack Layout after Exception of Interrupt
    
    //                 WITHOUT PRIVILEGE TRANSITION
    // D O     31              0           31              0
    // I F     ╠═══════╦═══════╣           ╠═══════╦═══════╣
    // R       ║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║ OLD       ║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║ OLD
    // E E     ╠═══════╬═══════╣ SS:ESP    ╠═══════╬═══════╣ SS:ESP
    // C X     ║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║   │       ║▒▒▒▒▒▒▒║▒▒▒▒▒▒▒║   │
    // T P     ╠═══════╩═══════╣◄──┘       ╠═══════╩═══════╣◄──┘
    // I A     ║  OLD EFLAGS   ║           ║  OLD EFLAGS   ║
    // O N     ╠═══════╦═══════╣           ╠═══════╦═══════╣
    // N S     ║▒▒▒▒▒▒▒║OLD CS ║ NEW       ║▒▒▒▒▒▒▒║OLD CS ║
    //   I     ╠═══════╩═══════╣ SS:ESP    ╠═══════╩═══════╣
    // │ O     ║    OLD EIP    ║   │       ║    OLD EIP    ║ NEW
    // │ N     ╠═══════════════╣◄──┘       ╠═══════════════╣ SS:ESP
    // │       ║               ║           ║  ERROR CODE   ║   │
    // ▼       ·               ·           ╠═══════════════╣◄──┘
    //         ·               ·           ║               ║
            
    //         WITHOUT ERROR CODE          WITH ERROR CODE
    

    //                 WITH PRIVILEGE TRANSITION
    // D O     31              0           31              0
    // I F     ╔═══════╦═══════╗◄──┐       ╔═══════╦═══════╗◄──┐
    // R       ║▒▒▒▒▒▒▒║OLD SS ║   │       ║▒▒▒▒▒▒▒║OLD SS ║   │
    // E E     ╠═══════╩═══════╣ SS:ESP    ╠═══════╩═══════╣ SS:ESP
    // C X     ║    OLD ESP    ║ FROM TSS  ║    OLD ESP    ║ FROM TSS
    // T P     ╠═══════════════╣           ╠═══════════════╣
    // I A     ║  OLD EFLAGS   ║           ║  OLD EFLAGS   ║
    // O N     ╠═══════╦═══════╣           ╠═══════╦═══════╣
    // N S     ║▒▒▒▒▒▒▒║OLD CS ║ NEW       ║▒▒▒▒▒▒▒║OLD CS ║
    //   I     ╠═══════╩═══════╣ SS:ESP    ╠═══════╩═══════╣
    // │ O     ║    OLD EIP    ║   │       ║    OLD EIP    ║ NEW
    // │ N     ╠═══════════════╣◄──┘       ╠═══════════════╣ SS:ESP
    // │       ║               ║           ║  ERROR CODE   ║   │
    // ▼       ·               ·           ╠═══════════════╣◄──┘
    //         ·               ·           ║               ║

    //         WITHOUT ERROR CODE          WITH ERROR CODE

    struct k_cpu_state * const currentCpuState = &pTab[currentProcId].cpuState;
    const struct k_cpu_state * const nextCpuState = &pTab[nextProcId].cpuState;
    const enum k_gdt_dpl currentDpl = pTab[currentProcId].dpl;
    const enum k_gdt_dpl nextDpl = pTab[nextProcId].dpl;

    if (currentDpl == DPL_0 && nextDpl == DPL_3) {
        // Switching to DPL3 process

        #warning probably a lot of these operations can be removed

        // Save kernel CPU state
        CPU_PUSH
        {
            currentCpuState->edi = STACK_VAL(32, 0);
            currentCpuState->esi = STACK_VAL(32, 4);
            currentCpuState->ebp = STACK_VAL(32, 8);
            currentCpuState->ebx = STACK_VAL(32, 16);
            currentCpuState->edx = STACK_VAL(32, 20);
            currentCpuState->ecx = STACK_VAL(32, 24);
            currentCpuState->eax = STACK_VAL(32, 28);
            currentCpuState->gs  = STACK_VAL(16, 32);
            currentCpuState->fs  = STACK_VAL(16, 34);
            currentCpuState->es  = STACK_VAL(16, 36);
            currentCpuState->ds  = STACK_VAL(16, 38);        
        }
        CPU_POP

        extern ptr_t k_proc_kernelRet;
        currentCpuState->eip = (uint_32)&k_proc_kernelRet;
        currentCpuState->esp = cur_esp;
        __asm__ volatile ("pushw %cs"); __asm__ volatile ("popw %[mem]" : [mem] "=m" (currentCpuState->cs));
        __asm__ volatile ("pushfl");    __asm__ volatile ("popl %[mem]" : [mem] "=m" (currentCpuState->eflags));
        __asm__ volatile ("pushw %ss"); __asm__ volatile ("popw %[mem]" : [mem] "=m" (currentCpuState->ss));

        // Restore next process CPU state

        // Set kernel stack pointer in TSS
        k_cpu_tss.esp0 = cur_esp;

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
        __asm__ volatile ("iret");

        // Kernel return location
        __asm__ volatile ("k_proc_kernelRet:");
    }
    else if (currentDpl == DPL_3 && nextDpl == DPL_0) {
        // Inside an interrupt - switching to kernel

        // k_proc_cpuSave(currentProcId);
        // k_proc_cpuRestore(nextProcId);
    }
    else {
        OOPS("Unexpected privilege jump");
    }
}

static void k_proc_prepareKernelProc() {
    pTab[0].state = PS_RUNNING;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_onTick();
    k_tmr_regMsHandler(k_proc_schedule_onTick);
}
