//
//  hal.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#include "hal.h"

#include <defs.h>
#include "proc/proc.h"
#include "cpu/cpu.h"
#include "gdt/gdt.h"
#include "paging/paging.h"
#include "int/irq.h"
#include "io/bus.h"
#include "mem/mem.h"
#include "pic/pic.h"

#include <error/panic.h>

extern void k_pic_configure();
extern void k_idt_loadDefault();
extern void k_proc_init();
extern void k_mem_init();

PRIVATE struct isrEntry {
    procId_t operator;
    void (*routine)();
} isrReg[DEV_IRQ_LIMIT];

__attribute__((naked)) void k_hal_prepare() {
    k_paging_prepare();
    __asm__ volatile ("jmp k_paging_start");
    __builtin_unreachable();
}

__attribute__((naked)) void k_paging_start_end() {
    __asm__ volatile ("jmp k_hal_prepare_end");
    __builtin_unreachable();
}

static void cleanISR(const size_t irq) {
    if (irq >= DEV_IRQ_LIMIT) {
        OOPS("IRQ out of range",);
        return;
    }

    isrReg[irq].operator = NONE_PROC_ID;
    isrReg[irq].routine = nullptr;
}

void k_hal_init(const struct k_krn_memMapEntry *memMap) {
    k_paging_init(memMap);
    k_idt_loadDefault();
    k_gdt_init();
    k_cpu_init();
    k_cpu_loadTaskRegister();

    k_pic_configure();

    k_proc_init();
    k_mem_init();

    for (size_t irq = 0; irq < DEV_IRQ_LIMIT; ++irq) {
        cleanISR(irq);
    }

    CPU_INTERRUPTS_ENABLE;
}

bool k_hal_isIRQAllowed(uint_8 num) {
    if (num >= DEV_IRQ_LIMIT) {
        return false;
    }
    if (num == SLAVE) {
        return false;
    }

    return true;
}

bool k_hal_isIRQRegistered(uint_8 num) {
    if (num >= DEV_IRQ_LIMIT) {
        return false;
    }
    if (isrReg[num].routine == nullptr) {
        return false;
    }

    return true;
}

enum gnwDriverError k_hal_install(const procId_t operator, const struct gnwDriverConfig driver) {
    if (!driver.isr) {
        return GDRE_ISR_MISSING;
    }
    if (!k_hal_isIRQAllowed(driver.irq)) {
        return GDRE_IRQ_INVALID;
    }
    if (k_hal_isIRQRegistered(driver.irq)) {
        return GDRE_IRQ_CONFLICT;
    }
    if (operator != KERNEL_PROC_ID) {
        if (!k_proc_isAlive(operator)) {
            return GDRE_INVALID_ARGUMENT;
        }
    }

    isrReg[driver.irq].operator = operator;
    isrReg[driver.irq].routine = driver.isr;
        
    extern void k_pic_enableIRQ(const enum k_dev_irq);
    k_pic_enableIRQ(driver.irq);
    
    return GDRE_NONE;
}

enum failReason_t {
    FAIL_REASON_IRQ_NOT_FOUND,
    FAIL_REASON_IRQ_ABOVE_LIMIT
};

#warning WORKAROUND - OOPS did not link

static void fail(const enum failReason_t reason) {
    switch (reason) {
    case FAIL_REASON_IRQ_NOT_FOUND:
        OOPS("Driver for specified IRQ not found",); break;
    case FAIL_REASON_IRQ_ABOVE_LIMIT:
        OOPS("Requested service of IRQ above limit",); break;
    default:
        OOPS("Unknown IRQ handling failure",); break;
    }
}

/*
    IRQ request global service routine
*/
void k_hal_irqHandle(const uint_8 irq) {
    /*
        * Checking if the requested IRQ is within accepted range
        * Checking if service routine for given IRQ is available
    */
    if (irq >= DEV_IRQ_LIMIT) {
        fail(FAIL_REASON_IRQ_ABOVE_LIMIT);
        return;
    }
    if (!isrReg[irq].routine) {
        fail(FAIL_REASON_IRQ_NOT_FOUND);
        return;
    }
    if (isrReg[irq].operator <= NONE_PROC_ID) {
        OOPS("Device operator inconsistency",);
    }
    
    if (isrReg[irq].operator == KERNEL_PROC_ID) {
        isrReg[irq].routine();
    } else {
        enum k_proc_error error = k_proc_callback_invoke_void(isrReg[irq].operator, isrReg[irq].routine);
        if (error != PE_NONE) {
            OOPS("Error invoking IRQ handler",);
        }
        error = k_proc_setPriority(isrReg[irq].operator, true);
        if (error != PE_NONE) {
            OOPS("Error invoking IRQ handler",);
        }
    }

    /*
        Finished servicing

        Enables interrupts and returns from interrupt

        NOTE: service routines MUST end with ret
    */

    /*
        Send EOI command to PIC
    */
    if (PIC_IRQ_IS_SLAVE(irq)) {
        k_bus_outb(BUS_PIC_SLAVE_COMMAND, PIC_EOI);
    }
    k_bus_outb(BUS_PIC_MASTER_COMMAND, PIC_EOI);
}
