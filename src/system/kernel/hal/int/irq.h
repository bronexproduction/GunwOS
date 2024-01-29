//
//  irq.h
//  GunwOS
//
//  Created by Artur Danielewski on 25.03.2020.
//

#ifndef IRQ_H
#define IRQ_H

#define DEV_IRQ_LIMIT 8

enum k_dev_irq {
                        // PIC      80368   Description
    PIT           = 0,  // 0        8	    Timer chip
    KEYBOARD      = 1,  // 1        9	    Keyboard
    SLAVE         =	2,  // 2        0Ah	    Cascade for controller 2 (IRQ 8-15)
    COM0          =	3,  // 3        0Bh	    Serial port 2
    COM1          =	4,  // 4        0Ch	    Serial port 1
    PAR1	      = 5,  // 5        0Dh	    Parallel port 2 in AT, reserved in PS/2 systems
    FLOPPY        =	6,  // 6        0Eh	    Diskette drive
    PAR0	      = 7,  // 7        0Fh	    Parallel port 1
    RTC           = 8,  // 8/0	    70h	    Real-time clock
    CVR           = 9,  // 9/1	    71h	    CGA vertical retrace (and other IRQ 2 devices)
    RESERVED_10   = 10, // 10/2	    72h	    Reserved
    RESERVED_11   = 11, // 11/3	    73h	    Reserved
    RESERVED_12   = 12, // 12/4	    74h	    Reserved in AT, auxiliary device on PS/2 systems
    FPU           = 13, // 13/5	    75h	    FPU interrupt
    HDD           = 14, // 14/6	    76h	    Hard disk controller
    RESERVED_15   = 15  // 15/7	    77h	    Reserved
};

#endif // IRQ_H
