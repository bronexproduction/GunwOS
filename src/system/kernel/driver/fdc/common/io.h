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

#define SET(BIT) (BIT)
#define CLEAR(BIT) ((BIT) & 0)

#define IS_SET(BIT, REG) ((BIT) & (REG))
#define IS_NOT_SET(BIT, REG) (!IS_SET((BIT), (REG)))

#define ALIGNED(VALUE, RANGE) (((VALUE) << __builtin_ctz(RANGE)) & (RANGE))

/*
    Routine for sending byte to FIFO
*/
enum fdc_opStatus pushData(const uint_16 base, const uint_8 data);

/*
    Routine for retrieving byte from FIFO
*/
enum fdc_opStatus pullData(const uint_16 base, uint_8 * const data);

#endif // IO_H
