//
//  io.h
//  GunwOS
//
//  Created by Artur Danielewski on 27.03.2020.
//  
//  Docs:
//  * Intel 82077AA Datasheet
//  * https://wiki.osdev.org/Floppy_Disk_Controller
//  
//  * FDC37C78 Floppy Disk Controller Data Sheet - SMSC
//  * http://ww1.microchip.com/downloads/en/DeviceDoc/37c78.pdf
//

#ifndef IO_H
#define IO_H

#include "fdctypes.h"

/*
    Routine for writing data to given register
*/
void pushReg(const uint_16 base, const enum fdc_bus_regOffset regOffset, const uint_8 data);

/*
    Routine for sending byte to FIFO
*/
enum fdc_opStatus pushData(const uint_16 base, const uint_8 data);

/*
    Routine for retrieving byte from FIFO
*/
enum fdc_opStatus pullData(const uint_16 base, uint_8 * const data);

#endif // IO_H
