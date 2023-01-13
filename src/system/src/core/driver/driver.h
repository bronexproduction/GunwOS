//
//  driver.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//

#ifndef DRIVER_H
#define DRIVER_H

/*
    Interrupt service routine macro

    Implement functions using this macro to prevent from incidentally forgeting the return label
*/
#define ISR_END {__asm__ volatile ("ret");};
#define ISR(CODE) __attribute__((naked)) static void isr() { CODE; ISR_END }

#endif // DRIVER_H
