//
//  data.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

#include "data.h"

bool present;
uint_8 driveCount;
struct fdc_fddPerf perf[FDC_FDD_SUPPORT_COUNT];
volatile bool irqRecv = 0;
