//
//  utl.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

#include "defaults.h"
#include "types.h"
#include "../../../../shared/log/log.h"
#include "../../../../kernel/timer/timer.h"
#include "../../../../../include/gunwscl.h"

extern struct gnwStorGeometry s_drv_fdc_uha_driveGeometry(const uint_8 index);

struct fdc_fddConfig fdc_configFor(const uint_32 driveID) {
    extern struct fdc_fddPerf *fdc_perf;
    extern uint_16 fdc_busBase;
    
    if (driveID >= FDC_FDD_SUPPORT_COUNT) {
        LOG_FATAL("Drive identifier over limit")
        __builtin_unreachable();
    }

    uint_16 base = fdc_busBase;
    uint_8 drive = driveID % FDC_FDD_SUPPORT_COUNT;
    uint_8 mt = BIT_MT;
    uint_8 mfm = BIT_MFM;
    struct fdc_fddPerf perf = fdc_perf[driveID];
    struct fdc_fddFormat format;
    format.gpl = 0x1b;
    format.sys = s_drv_fdc_uha_driveGeometry(driveID);

    return (struct fdc_fddConfig) { base, drive, mt, mfm, perf, format };

    #warning not implemented yet
}

enum fdc_sectSize fdc_sectSizeFor(const uint_16 sectBytes) {
    if (sectBytes == 128)   return SECT_128B;
    if (sectBytes == 256)   return SECT_256B;
    if (sectBytes == 512)   return SECT_512B;
    if (sectBytes == 1024)  return SECT_1KB;
    if (sectBytes == 2048)  return SECT_2KB;
    if (sectBytes == 4096)  return SECT_4KB;
    if (sectBytes == 8192)  return SECT_8KB;
    if (sectBytes == 16384) return SECT_16KB;
    
    return SECT_UNKNOWN;
}

/*
    Wait for interrupt

    Returns 1 if the interrupt flag gets set, 0 otherwise

    NOTE: fdc_irqRecv flag should be cleared prior to waitForInterrupt() call
*/
uint_8 fdc_waitForInterrupt(const time_t ms) {
    extern uint_8 fdc_irqRecv;

    for (int i = 0; i < ms; ++i) {
        if (fdc_irqRecv) { 
            fdc_irqRecv = 0; 
            return 1; 
        }

        sleepms(1);
    }

    return 0;
}
