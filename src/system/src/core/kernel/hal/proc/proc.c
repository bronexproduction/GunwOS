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

#define CPU_PUSH { \
    __asm__ volatile ("pushw %ds"); \
    __asm__ volatile ("pushw %es"); \
    __asm__ volatile ("pushw %fs"); \
    __asm__ volatile ("pushw %gs"); \
    __asm__ volatile ("pushal"); \
}

#define CPU_POP { \
    __asm__ volatile ("popal"); \
    __asm__ volatile ("popw %gs"); \
    __asm__ volatile ("popw %fs"); \
    __asm__ volatile ("popw %es"); \
    __asm__ volatile ("popw %ds"); \
}

struct k_proc_process pTab[MAX_PROC];
size_t k_proc_currentProcId = 0;

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

void k_proc_switch(const size_t nextProcId, const bool isr) {
    pTab[k_proc_currentProcId].state = PS_READY;
    pTab[nextProcId].state = PS_RUNNING;

    // ISR puts 12 bytes on the stack?

    if (isr) {
        // inside an ISR
        // switching from R3 to kernel
    } else {
        // not inside an ISR
        // switching from kernel to R3
        CRITICAL_SECTION_END;
        __asm__ volatile ("iret");
    }
}

void k_proc_cpuSave() {
    CPU_PUSH
    CPU_PUSH
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.edi));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.esi));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ebp));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.esp)); // it may not be the ESP we need
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ebx));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.edx));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ecx));
    __asm__ volatile ("popl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.eax));
    __asm__ volatile ("popw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.gs));
    __asm__ volatile ("popw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.fs));
    __asm__ volatile ("popw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.es));
    __asm__ volatile ("popw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ds));
    CPU_POP
    
    #warning TO BE IMPLEMENTED
    // /*
    //     Instruction pointer
    // */
    // uint_32 eip;
    
    // /*
    //     Status register
    // */
    // uint_32 eflags;
}

void k_proc_cpuRestore() {
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ds));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.es));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.fs));
    __asm__ volatile ("pushw %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.gs));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.eax));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ecx));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.edx));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ebx));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.esp)); // it may not be the ESP we need
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.ebp));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.esi));
    __asm__ volatile ("pushl %[mem]" : [mem] "=m" (pTab[k_proc_currentProcId].cpuState.edi));
    CPU_POP
        
    #warning TO BE IMPLEMENTED
    // /*
    //     Instruction pointer
    // */
    // uint_32 eip;
    
    // /*
    //     Status register
    // */
    // uint_32 eflags;
}

void __attribute__ ((cdecl)) k_proc_updateEAX(const uint_32 eax) {
    pTab[k_proc_currentProcId].cpuState.eax = eax;
}

static void k_proc_prepareKernelProc() {
    pTab[0].state = PS_RUNNING;
}

void k_proc_init() {
    k_proc_prepareKernelProc();

    extern void k_proc_schedule_onTick();
    k_tmr_regMsHandler(k_proc_schedule_onTick);
}
