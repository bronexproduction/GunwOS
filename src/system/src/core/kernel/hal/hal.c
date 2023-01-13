//
//  hal.c
//  GunwOS
//
//  Created by Artur Danielewski on 21.01.2021.
//

#include <stdgunw/types.h>
#include <gunwdrv.h>
#include "int/irq.h"
#include "io/bus.h"
#include "pic/pic.h"

#include "../../log/log.h"

extern void k_pic_configure();
extern void k_idt_loadDefault(const char codeSegOffset);

void k_hal_init(const char codeSegOffset) {

    // TODO: CPU configuration? seen on http://www.brokenthorn.com/Resources/OSDev20.html
    // TODO: move GDT configuration from the boot loader
    k_idt_loadDefault(codeSegOffset);

    k_pic_configure();

    __asm__ volatile ("sti");
}

struct driver {
    /*
        Driver descriptor
    */
    struct gnwDriverDesc desc;

    /*
        Initialization mark
    */
    uint_8 initialized;

    /*
        Driver start mark
    */
    uint_8 started;
};

static struct driver driverReg[DEV_IRQ_LIMIT];

enum gnwDriverError k_hal_install(struct gnwDriverDesc driver) {
    struct driver drv = { driver, 0, 0 };

    if (driverReg[driver.irq].desc.isr) {
        LOG_FATAL("IRQ conflict")
        return IRQ_CONFLICT;
    }

    drv.initialized = driver.init ? driver.init() : 1;
    if (drv.initialized) {
        driverReg[driver.irq] = drv;
        
        extern void k_pic_enableIRQ(const enum k_dev_irq);
        k_pic_enableIRQ(driver.irq);
    }
    else {
        LOG_ERR("Driver init failed");
        return UNINITIALIZED;
    }

    return NO_ERROR;
}

enum gnwDriverError k_hal_start(struct gnwDriverDesc driver) {
    if (!driverReg[driver.irq].desc.isr) {
        LOG_FATAL("Trying to start uninitialized driver");
        return UNINITIALIZED;
    }
    
    driverReg[driver.irq].started = driverReg[driver.irq].desc.start ? driverReg[driver.irq].desc.start() : 1;

    return driverReg[driver.irq].started ? NO_ERROR : START_FAILED;
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
    void (*isr)() = driverReg[irq].desc.isr;
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
    LOG_DEBUG("Driver for specified IRQ not found");
    __asm__ volatile ("jmp k_hal_irqHandle_end");

    /*
        Handling error - IRQ number above limit
    */
    __asm__ volatile ("k_hal_irqHandle_irqAboveLimitFailure:");
    LOG_FATAL("Requested service of IRQ above limit");
    __asm__ volatile ("jmp k_hal_irqHandle_end");
}
