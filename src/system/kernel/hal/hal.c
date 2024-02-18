//
//  hal.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#include "hal.h"

#include <defs.h>
#include "cpu/cpu.h"
#include "gdt/gdt.h"
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
    size_t devId;
    void (*routine)();
} isrReg[DEV_IRQ_LIMIT];

const size_t *k_hal_servicedDevIdPtr;

void k_hal_init() {
    k_cpu_init();

    k_gdt_init();
    k_cpu_loadTaskRegister();
    k_idt_loadDefault();

    k_pic_configure();

    k_proc_init();
    k_mem_init();

    CPU_INTERRUPTS_ENABLE;
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

enum gnwDriverError k_hal_install(const size_t devId, const struct gnwDriverConfig driver) {
    if (!driver.isr) {
        return GDRE_ISR_MISSING;
    }
    if (k_hal_isIRQRegistered(driver.irq)) {
        return GDRE_IRQ_CONFLICT;
    }

    isrReg[driver.irq].devId = devId;
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
    } else if (!isrReg[irq].routine) {
        fail(FAIL_REASON_IRQ_NOT_FOUND);
    } else {
        k_hal_servicedDevIdPtr = &isrReg[irq].devId;
        isrReg[irq].routine();
    }

    /*
        Finished servicing

        Enables interrupts and returns from interrupt

        NOTE: service routines MUST end with ret
    */

    /*
        Clearing currently serviced device identifier
    */
    k_hal_servicedDevIdPtr = nullptr;

    /*
        Send EOI command to PIC
    */
    if (irq > 7) {
        k_bus_outb(BUS_PIC_SLAVE_COMMAND, PIC_EOI);
    }
    k_bus_outb(BUS_PIC_MASTER_COMMAND, PIC_EOI);
}
