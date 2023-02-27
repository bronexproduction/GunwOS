//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 25.01.2023.
//

#include "proc.h"

#include <stdgunw/mem.h>

#include "../../schedule/schedule.h"
#include "../criticalsec/criticalsec.h"
#include "../../timer/timer.h"
#include "../../error/panic.h"
#include "../cpu/cpu.h"

register uint_32 cur_esp __asm__ ("esp");

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

    pTab[pIndex].cpuState.esp = (uint_32)descriptor->stack;
    pTab[pIndex].cpuState.eip = (uint_32)descriptor->img;

    k_proc_schedule_didSpawn(pIndex);

    return PE_NONE;
}

/*
    Saving current process CPU state
    in case of an interrupt

    For more information
    refer to Intel i386 Programmer's Reference Manual (1986)
    section 9.6.1 Interrupt Procedures
*/
static void k_proc_cpuSave(const uint_32 esp, const size_t procId) {

    // NO ERROR CODE
    //
    // Exceptions with error code
    // currently not supported

    CPU_PUSH
    {
        #warning TO BE IMPLEMENTED

        if (procId) {
            struct k_cpu_state *cpuState = &pTab[procId].cpuState;
        
            cpuState->edi = STACK_VAL(32, 0);
            cpuState->esi = STACK_VAL(32, 4);
            cpuState->ebp = STACK_VAL(32, 8);
            cpuState->ebx = STACK_VAL(32, 16);
            cpuState->edx = STACK_VAL(32, 20);
            cpuState->ecx = STACK_VAL(32, 24);
            cpuState->eax = STACK_VAL(32, 28);
            cpuState->gs  = STACK_VAL(16, 32);
            cpuState->fs  = STACK_VAL(16, 34);
            cpuState->es  = STACK_VAL(16, 36);
            cpuState->ds  = STACK_VAL(16, 38);

            // Stack with privilege transition

            cpuState->eip = *(uint_32 *)esp;
            cpuState->cs  = *(uint_16 *)(esp + 4);
            cpuState->eflags = *(uint_32 *)(esp + 8);
            cpuState->esp = *(uint_32 *)(esp + 12);
            cpuState->ss  = *(uint_16 *)(esp + 16);
        }
    }
    CPU_POP
}

/*
    Restoring previous process CPU state
    called just before return from interrupt

    For more information
    refer to Intel i386 Programmer's Reference Manual (1986)
    section 9.6.1 Interrupt Procedures
*/
static void k_proc_cpuRestore(const uint_32 esp, const size_t procId) {

    struct k_cpu_state *cpuState = &pTab[procId].cpuState;

    if (procId) {
        // not inside an ISR
        // switching from kernel to R3
    } else {
        // inside an ISR
        // switching from R3 to kernel
    }

    if (procId) {
        // With privilege transition

        // cpuState->esp = *(uint_32 *)(esp + 12);
        // cpuState->ss  = *(uint_16 *)(esp + 16);
    } else {
        // No privilege transition

        // *(uint_32 *)esp = cpuState->eip;
        // *(uint_16 *)(esp + 4) = cpuState->cs;
        // *(uint_32 *)(esp + 8) = cpuState->eflags;
    }

    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (cpuState->ds));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (cpuState->es));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (cpuState->fs));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (cpuState->gs));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->eax));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->ecx));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->edx));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->ebx));
    __asm__ volatile ("pushl %esp");
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->ebp));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->esi));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (cpuState->edi));
    CPU_POP
}

/*
    Switching between processes

    For more information
    refer to Intel i386 Programmer's Reference Manual (1986)
    section 7.5 Task Switching
*/
void k_proc_switch(const uint_32 esp, const size_t currentProcId, const size_t nextProcId) {
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

    k_proc_cpuSave(esp, currentProcId);
    k_proc_cpuRestore(esp, nextProcId);

    if (nextProcId) {
        CRITICAL_SECTION_END;
        __asm__ volatile ("iret");
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
