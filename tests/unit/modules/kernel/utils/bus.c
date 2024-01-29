//
//  bus.c
//  GunwOS
//
//  Created by Artur Danielewski on 08.01.2024.
//

#include <hal/io/bus.h>

/*
    Callable variants of bus input/output functions
*/

void bus_outb(uint_16 port, uint_8 val) {
    k_bus_outb(port, val);
}
