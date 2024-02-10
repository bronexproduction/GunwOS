//
//  io.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//  
//  Docs:
//  * Intel 82077AA Datasheet
//  * https://wiki.osdev.org/Floppy_Disk_Controller
//  
//  * FDC37C78 Floppy Disk Controller Data Sheet - SMSC
//  * http://ww1.microchip.com/downloads/en/DeviceDoc/37c78.pdf
//

#include "io.h"
#include <gunwctrl.h>
#include "../utl/utl.h"
#include <hal/io/bus.h>

void pushReg(const uint_16 base, const enum fdc_bus_regOffset regOffset, const uint_8 data) {
    k_bus_outb(base + regOffset, data);
    extern void fdc_sleepms(const size_t);
    fdc_sleepms(1);
}

enum fdc_opStatus pushData(const uint_16 base, const uint_8 data) {
    uint_8 status = waitForCommandPhase(base);
    if (!status) {
        return OPSTATUS_NOT_READY;
    }

    pushReg(base, REG_DATA, data);
    return OPSTATUS_OK;
}

enum fdc_opStatus pullData(const uint_16 base, uint_8 * const data) {
    if (!data) return OPSTATUS_INVPARAM;

    if (!waitForResultPhase(base)) {
        return OPSTATUS_NOT_READY;
    }

    *data = k_bus_inb(base + REG_DATA);
    return OPSTATUS_OK;
}
