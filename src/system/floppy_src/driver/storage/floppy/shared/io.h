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

#ifndef FDC_IO_H
#define FDC_IO_H

#include "../../../../../include/gunwio.h"
#include "../../../../../include/gunwscl.h"
#include "types.h"

// These are mostly pointless but increases readability
#define SET(BIT) (BIT)
#define CLEAR(BIT) ((BIT) & 0)

#define IS_SET(BIT, REG) ((BIT) & (REG))
#define IS_NOT_SET(BIT, REG) (!IS_SET((BIT), (REG)))

#define ALIGNED(VALUE, RANGE) (((VALUE) << __builtin_ctz(RANGE)) & (RANGE))

/*
    Routine for sending byte to register at specified offset
*/
static inline void pushReg(const uint_16 base, const enum fdc_bus_regOffset regOffset, const uint_8 data) {
    wrb(base + regOffset, data);
    sleepms(1);
}

/*
    Routine for sending byte to FIFO
*/
static inline enum k_fdc_opStatus pushData(const uint_16 base, const uint_8 data) {
    extern uint_8 fdc_waitForCommandPhase(const uint_16);
    uint_8 status = fdc_waitForCommandPhase(base);
    if (!status) {
        return OPSTATUS_NOT_READY;
    }

    pushReg(base, REG_DATA, data);
    return OPSTATUS_OK;
}

/*
    Routine for retrieving byte from FIFO
*/
static inline enum k_fdc_opStatus pullData(const uint_16 base, uint_8 * const data) {
    if (!data) return OPSTATUS_INVPARAM;

    extern uint_8 fdc_waitForResultPhase(const uint_16);
    if (!fdc_waitForResultPhase(base)) {
        return OPSTATUS_NOT_READY;
    }

    *data = rdb(base + REG_DATA);
    #warning does it even work on read?
    #warning find other way to wait
    // k_io_wait();
    return OPSTATUS_OK;
}

#endif // FDC_IO_H
