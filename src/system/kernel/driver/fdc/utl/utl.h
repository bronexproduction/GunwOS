//
//  utl.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#ifndef UTL_H
#define UTL_H

#include "../common/fdctypes.h"

struct fdc_fddConfig configFor(const uint_32 driveID);
enum fdc_sectSize sectSizeFor(const uint_16 sectBytes);
struct gnwStorGeometry driveGeometry(const uint_8 index);

/*
    Wait for interrupt

    Returns 1 if the interrupt flag gets set, 0 otherwise

    NOTE: fdc_irqRecv flag should be cleared prior to waitForInterrupt() call
*/
uint_8 waitForInterrupt(const time_t ms);

/*
    Indicates data being ready to read from FIFO in non-DMA mode
*/
bool isReadyForNonDMARead(const uint_16 base);

#endif // UTL_H
