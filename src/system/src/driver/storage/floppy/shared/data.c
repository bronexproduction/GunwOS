//
//  data.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

#include "defaults.h"
#include "types.h"

/*
    Floppy controller base bus address
*/
uint_16 fdc_busBase = 0x3F0;

/*
    Mark controller as present

    1 - detected
    0 - not detected
*/
uint_8 fdc_present;

/*
    Mask of detected drives
*/
uint_8 fdc_drives;

/*

    Performance configuration per FDD
*/
struct fdc_fddPerf fdc_perf[FDC_FDD_SUPPORT_COUNT];

/*
    IRQ reception mark

    The generation of INT determines the beginning of the result phase
*/
volatile uint_8 fdc_irqRecv = 0;
