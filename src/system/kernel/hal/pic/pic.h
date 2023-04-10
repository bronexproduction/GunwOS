//
//  pic.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//
//  Some docs:
//  https://www.eeeguide.com/programming-8259/
//  https://pdos.csail.mit.edu/6.828/2008/readings/hardware/8259A.pdf
//

#ifndef PIC_H
#define PIC_H

/*
    Master PIC command word register
*/
#define BUS_PIC_MASTER_COMMAND  0x20

/*
    Master PIC data word register
*/
#define BUS_PIC_MASTER_DATA     0x21

/*
    Slave PIC command word register
*/
#define BUS_PIC_SLAVE_COMMAND   0xA0

/*
    Slave PIC data word register
*/
#define BUS_PIC_SLAVE_DATA      0xA1

/*
    Initialization command word 1 (ICW1)

    A write command issued to the 8259 with A0 = 0 and D4 = 1 is interpreted as ICW1

    MSB                                                                                 LSB
    |         D7 - D5         | D4 | D3            | D2         | D1          | D0          |
    | A7 - A5 of interrupt    | 1  | Trigger mode  | Call addr  | Mode        | ICW4 needed |
    | (MCS - 80/85 mode only) |    | 0 - edge      | interval   | 0 - Cascade | 0 - No      |
    |                         |    | 1 - level     | 0 - 8 bits | 1 - Single  | 1 - Yes     |
    |                         |    |               | 1 - 4 bits |             |             |
*/

#define PIC_ICW1_ICW4	        0x01	/* ICW4 needed (D0) */
#define PIC_ICW1_MODE_SINGLE    0x02    /* Mode (D1) */
#define PIC_ICW1_INTERVAL_4     0x04    /* Interval between the interrupt vector locations (D2) */      
#define PIC_ICW1_TRIGGER_LEVEL  0x08    /* Interrupt trigger mode (D3) */
#define PIC_ICW1_INIT	        0x10	/* Initialization mark (D4) */

/*
    Initialization command word 2 (ICW2)

    Contains address of interrupt vector
    No separate bits specified
*/

/*
    Initialization command word 3 (ICW3)

    ICW3 is required only if there is more than one 8259 in the system and if they are cascaded.

    For master PIC:
    Bit set informs that slave PIC is connected to the selected IRQ (bits and IRQs ordered 7-0 - MSB first)

    For slave PIC:
    MSB                                                      LSB
    | D7 | D6 | D5 | D4 | D3 | D2 - D0                         |
    | 0  | 0  | 0  | 0  | 0  | Master IRQ slave is attached to |

    Source:
    https://stanislavs.org/helppc/8259.html

    Slave PIC ICW3 should equal 0x02
    to set its identity to cascade

    Source:
    https://wiki.osdev.org/8259_PIC
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
*/

#define PIC_ICW3_MASTER_IRQ2    0x04 /* Pin slave PIC is connected to - IRQ2 bit */
#define PIC_ICW3_SLAVE_CASCADE  0x02 /* Tells slave PIC its cascade identity (https://wiki.osdev.org/8259_PIC) */

/*
    Initialization command word 4 (ICW4)

    MSB                                                                                           LSB
    | D7 | D6 | D5 | D4            | D3            | D2            | D1           | D0              |
    | 0  | 0  | 0  | Special fully | Buffered mode | Buffered mode | Auto EOI     | CPU type        |
    |    |    |    | nested mode   | 0 - false     | device type   | 0 - disabled | 0 - MCS - 80/85 |
    |    |    |    | 0 - disabled  | 1 - true      | 0 - slave     | 1 - enabled  | 1 - 8086/8088   |
    |    |    |    | 1 - enabled   |               | 1 - master    |              |                 |
*/

#define PIC_ICW4_8086_88    0x01    /* CPU type (D0) */
#define PIC_ICW4_AUTO_EOI   0x02    /* Automatic end of interrupt (D1) */
#define PIC_ICW4_MASTER     0x04    /* Buffered mode device type (D2) */
#define PIC_ICW4_BUFFERED   0x08    /* Buffered mode (D3) */
#define PIC_ICW4_NESTED     0x10    /* Special fully nested mode (D4) */

/*
    Additional commands
*/

#define PIC_EOI             0x20    // End of interrupt

#endif // PIC_H
