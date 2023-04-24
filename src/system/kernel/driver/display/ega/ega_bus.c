//
//  ega_bus.c
//  GunwOS
//
//  Created by Artur Danielewski on 24.03.2023.
//

#include "ega_bus.h"

uint_16 bus_addr(uint_16 baseAddr, enum gnwDeviceUHA_display_format format) {
    if (!GDD_FMT_ISGRAP(format)) {
        return baseAddr;
    }

    if ((baseAddr & BUS_ADDR_MONOCHROME_MARK_MASK) != BUS_ADDR_MONOCHROME_MARK) {
        return baseAddr;
    }
    
    return baseAddr + BUS_ADDR_COLOR_OFFSET;
}
