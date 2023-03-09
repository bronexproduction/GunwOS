//
//  hal.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#include "hal.h"

#include <stdgunw/defs.h>
#include <stdgunw/mem.h>
#include "cpu/cpu.h"
#include "gdt/gdt.h"
#include "int/irq.h"
#include "io/bus.h"
#include "pic/pic.h"

#include <error/panic.h>

extern void k_pic_configure();
extern void k_idt_loadDefault();
extern void k_proc_init();

static struct isrEntry {
    size_t devId;
    void (*routine)();
} isrReg[DEV_IRQ_LIMIT];

void k_hal_init() {
    memnull(isrReg, sizeof(struct isrEntry) * DEV_IRQ_LIMIT);

    k_cpu_init();
    #warning TODO: move GDT configuration from the boot loader

    k_gdt_init();
    k_cpu_loadTaskRegister();
    k_idt_loadDefault();

    k_pic_configure();

    k_proc_init();

    __asm__ volatile ("sti");
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

enum gnwDriverError k_hal_install(const size_t devId, struct gnwDriverConfig driver) {
    if (!driver.isr) {
        return ISR_MISSING;
    }
    if (k_hal_isIRQRegistered(driver.irq)) {
        return IRQ_CONFLICT;
    }

    isrReg[driver.irq].devId = devId;
    isrReg[driver.irq].routine = driver.isr;
        
    extern void k_pic_enableIRQ(const enum k_dev_irq);
    k_pic_enableIRQ(driver.irq);
    
    return NO_ERROR;
}

enum failReason_t {
    FAIL_REASON_IRQ_NOT_FOUND,
    FAIL_REASON_IRQ_ABOVE_LIMIT
};

#warning WORKAROUND - OOPS did not link

static void fail(const enum failReason_t reason) {
    switch (reason) {
    case FAIL_REASON_IRQ_NOT_FOUND:
        OOPS("Driver for specified IRQ not found"); break;
    case FAIL_REASON_IRQ_ABOVE_LIMIT:
        OOPS("Requested service of IRQ above limit"); break;
    default:
        OOPS("Unknown IRQ handling failure"); break;
    }
}

/*
    IRQ request global service routine

    NOTE: IRQ number has to be put in EAX register
    before making jump to k_hal_irqHandle label
*/
__attribute__((naked)) void k_hal_irqHandle() {
    /*
        IRQ number to be serviced   
    */
    register uint_8 irq __asm__ ("eax");

    /*
        Checking if the requested IRQ is within accepted range
    */                             
    __asm__ volatile ("cmp %%ebx, %%eax" : : "b" (DEV_IRQ_LIMIT));
    __asm__ volatile ("jae k_hal_irqHandle_irqAboveLimitFailure");

    /*
        Checking if service routine for given IRQ is available
    */
    void (*isr)() = isrReg[irq].routine;
    if (!isr) {
        __asm__ volatile ("jmp k_hal_irqHandle_irqServiceRoutineUnavailable");
    }

    /*
        Calling service routine associated with requested IRQ
    */
    __asm__ volatile ("call *%0" : : "r" (isr));
    
    /*
        Finished servicing

        Enables interrupts and returns from interrupt

        NOTE: service routines MUST end with ret
    */

    __asm__ volatile ("k_hal_irqHandle_end:");

    /*
        Send EOI command to PIC
    */
    if (irq > 7) {
        k_bus_outb(BUS_PIC_SLAVE_COMMAND, PIC_EOI);
    }
    k_bus_outb(BUS_PIC_MASTER_COMMAND, PIC_EOI);

    __asm__ volatile ("ret");

    /*
        Handling IRQ service routine unavailable 
    */
    __asm__ volatile ("k_hal_irqHandle_irqServiceRoutineUnavailable:");
    fail(FAIL_REASON_IRQ_NOT_FOUND);
    __asm__ volatile ("jmp k_hal_irqHandle_end");

    /*
        Handling error - IRQ number above limit
    */
    __asm__ volatile ("k_hal_irqHandle_irqAboveLimitFailure:");
    fail(FAIL_REASON_IRQ_ABOVE_LIMIT);
    __asm__ volatile ("jmp k_hal_irqHandle_end");
}
