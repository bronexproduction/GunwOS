//
//  pic.c
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//
//  Some docs:
//  https://www.eeeguide.com/programming-8259/
//  https://pdos.csail.mit.edu/6.828/2008/readings/hardware/8259A.pdf
//  https://wiki.osdev.org/8259_PIC
//

#include "pic.h"

#include <hal/io/bus.h>
#include <hal/int/irq.h>
#include <error/panic.h>

static uint_8 init() {

    /*
        ICW1
    */
	k_bus_outb(BUS_PIC_MASTER_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
    k_bus_outb(BUS_PIC_SLAVE_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);

    /*
        ICW2 
        
        Interrupts are remapped to start from 32 
    */
	k_bus_outb(BUS_PIC_MASTER_DATA, 0x20);
	k_bus_outb(BUS_PIC_SLAVE_DATA, 0x28);

    /*
        ICW3

        For cascade mode
    */
	k_bus_outb(BUS_PIC_MASTER_DATA, PIC_ICW3_MASTER_IRQ2);
	k_bus_outb(BUS_PIC_SLAVE_DATA, PIC_ICW3_SLAVE_CASCADE);

    /*
        ICW4
    */
	k_bus_outb(BUS_PIC_MASTER_DATA, PIC_ICW4_8086_88);
	k_bus_outb(BUS_PIC_SLAVE_DATA, PIC_ICW4_8086_88);

    return 1;
}

static void disableIRQs() {
    
    k_bus_outb(BUS_PIC_MASTER_DATA, 0xFF);
    k_bus_outb(BUS_PIC_SLAVE_DATA, 0xFF);
}

void k_pic_configure() {

    init();
    disableIRQs();
}

void k_pic_enableIRQ(const enum k_dev_irq num) {

    if (num > 15) OOPS("Attempt to enable IRQ above supported range",);
    if (num > 7) OOPS("IRQ range not implemented yet",);

    #warning IRQ > 7 not supported
    uint_16 port = BUS_PIC_MASTER_DATA;

    uint_8 mask = k_bus_inb(port);
    mask = ~(1 << num) & mask;
    k_bus_outb(port, mask);
}
