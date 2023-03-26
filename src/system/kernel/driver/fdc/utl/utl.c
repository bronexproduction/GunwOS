//
//  utl.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//  

#include "utl.h"
#include "../common/defaults.h"
#include <error/panic.h>
#include <gunwctrl.h>
#include <driver/gunwbus.h>
#include "../common/io.h"

struct fdc_fddConfig configFor(const uint_32 driveID) {
    extern struct fdc_fddPerf *fdc_perf;
    extern uint_16 fdc_busBase;
    
    if (driveID >= FDC_FDD_SUPPORT_COUNT) {
        OOPS("Drive identifier over limit");
        __builtin_unreachable();
    }

    uint_16 base = fdc_busBase;
    uint_8 drive = driveID % FDC_FDD_SUPPORT_COUNT;
    uint_8 mt = BIT_MT;
    uint_8 mfm = BIT_MFM;
    struct fdc_fddPerf perf = fdc_perf[driveID];
    struct fdc_fddFormat format;
    format.gpl = 0x1b;
    format.sys = driveGeometry(driveID);

    return (struct fdc_fddConfig) { base, drive, mt, mfm, perf, format };

    #warning not implemented yet
}

enum fdc_sectSize sectSizeFor(const uint_16 sectBytes) {
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

struct gnwStorGeometry driveGeometry(const uint_8 index) {
    struct gnwStorGeometry geometry = {0};

    const size_t lba = chs2lba(geometry.chs.c, geometry.chs.h, geometry.chs.s, geometry.chs.h, geometry.chs.s);
    if (!lba) {
        return geometry;
    }

    // 3.5" 1.44 MB floppies supported only
    geometry.chs.c = 80;
    geometry.chs.h = 2;
    geometry.chs.s = 18;
    geometry.lba = lba;
    geometry.sectSize = 512;

    return geometry;
}

uint_8 waitForInterrupt(const time_t ms) {
    extern uint_8 fdc_irqRecv;

    for (time_t i = 0; i < ms; ++i) {
        if (fdc_irqRecv) { 
            fdc_irqRecv = 0; 
            return 1; 
        }

        sleepms(1);
    }

    return 0;
}

bool isReadyForNonDMARead(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_RQM, msr) && IS_SET(BIT_MSR_DIO, msr) && IS_SET(BIT_MSR_NDMA, msr);
}
