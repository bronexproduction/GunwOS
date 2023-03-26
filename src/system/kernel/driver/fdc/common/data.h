//
//  data.h
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#ifndef DATA_H
#define DATA_H

#include "fdctypes.h"
#include "defaults.h"

/*
    Floppy controller base bus address
*/
#define FDC_BUS_BASE 0x3F0

/*
    Mark controller as present

    1 - detected
    0 - not detected
*/
extern bool present;

/*
    Mask of detected drives
*/
extern uint_8 driveCount;

/*

    Performance configuration per FDD
*/
extern struct fdc_fddPerf perf[FDC_FDD_SUPPORT_COUNT];

/*
    IRQ reception mark

    The generation of INT determines the beginning of the result phase
*/
extern volatile bool irqRecv;

#endif // DATA_H
