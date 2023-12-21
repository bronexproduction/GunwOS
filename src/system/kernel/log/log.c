//
//  log.c
//  GunwOS
//
//  Created by Artur Danielewski on 20.12.2023.
//

#include "log.h"
#include <string.h>
#include <hal/io/bus.h>

/*
    Temporary implementation copied from OSDev Wiki
*/

#define PORT 0x3f8          // COM1

static int isTransmitEmpty() {
   return k_bus_inb(PORT + 5) & 0x20;
}
 
static void writeSerial(char a) {
   while (isTransmitEmpty() == 0);
 
   k_bus_outb(PORT,a);
}

void k_log_init() {
   k_bus_outb(PORT + 1, 0x00);    // Disable all interrupts
   k_bus_outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   k_bus_outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   k_bus_outb(PORT + 1, 0x00);    //                  (hi byte)
   k_bus_outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   k_bus_outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   k_bus_outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   k_bus_outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   k_bus_outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(k_bus_inb(PORT + 0) != 0xAE) {
      return;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   k_bus_outb(PORT + 4, 0x0F);
}

void k_log_log(const char * const msg) {
    const size_t msgLen = strlen(msg);
    for (size_t i = 0; i < msgLen; ++i) {
        writeSerial(msg[i]);
    }
}
