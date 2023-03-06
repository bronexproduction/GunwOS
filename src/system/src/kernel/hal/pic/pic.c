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

#include <gunwbus.h>

#include "pic.h"

#include <hal/int/irq.h>
#include <hal/io/io.h>
#include <log/log.h>
 
extern int k_trm_putc(const char c);
extern int k_trm_puts(const char * const s);
extern int k_trm_putun(uint_32 i);

static uint_8 init() {

    /*
        ICW1
    */
	wrb(BUS_PIC_MASTER_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	k_io_wait();
    wrb(BUS_PIC_SLAVE_COMMAND, PIC_ICW1_INIT | PIC_ICW1_ICW4);
	k_io_wait();

    /*
        ICW2 
        
        Interrupts are remapped to start from 32 
    */
	wrb(BUS_PIC_MASTER_DATA, 0x20);
	k_io_wait();
	wrb(BUS_PIC_SLAVE_DATA, 0x28);
	k_io_wait();

    /*
        ICW3

        For cascade mode
    */
	wrb(BUS_PIC_MASTER_DATA, PIC_ICW3_MASTER_IRQ2);
	k_io_wait();
	wrb(BUS_PIC_SLAVE_DATA, PIC_ICW3_SLAVE_CASCADE);
	k_io_wait();

    /*
        ICW4
    */
	wrb(BUS_PIC_MASTER_DATA, PIC_ICW4_8086_88);
	k_io_wait();
	wrb(BUS_PIC_SLAVE_DATA, PIC_ICW4_8086_88);
	k_io_wait();

    return 1;
}

static void disableIRQs() {
    
    wrb(BUS_PIC_MASTER_DATA, 0xFF);
    k_io_wait();
    wrb(BUS_PIC_SLAVE_DATA, 0xFF);
    k_io_wait();
}

void k_pic_configure() {

    init();
    disableIRQs();
}

void k_pic_enableIRQ(const enum k_dev_irq num) {

    if (num > 15) LOG_FATAL("Attempt to enable IRQ above supported range");
    if (num > 7) LOG_FATAL("IRQ range not implemented yet");

    #warning IRQ > 7 not supported
    uint_16 port = BUS_PIC_MASTER_DATA;

    uint_8 mask = rdb(port);
    mask = ~(1 << num) & mask;
    wrb(port, mask);
    k_io_wait();
}
