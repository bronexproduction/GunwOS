//
//  utl.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//  

#include "utl.h"
#include <gunwbus.h>
#include <gunwctrl.h>
#include <mem.h>
#include <gunwbus.h>
#include <error/panic.h>
#include "../common/data.h"
#include "../common/io.h"
#include "../proc/proc.h"

struct fdc_fddConfig configFor(const uint_32 driveID) {
    struct fdc_fddConfig result;
    memzero(&result, sizeof(struct fdc_fddConfig));

    if (driveID >= FDC_FDD_SUPPORT_COUNT) {
        OOPS("Drive identifier over limit", result);
    }

    result.base = FDC_BUS_BASE;
    result.drive = driveID % FDC_FDD_SUPPORT_COUNT;
    result.mt = BIT_MT;
    result.mfm = BIT_MFM;
    result.perf = perf[driveID];
    result.format.gpl = 0x1b;
    result.format.sys = driveGeometry(driveID);

    return result;

    #warning not implemented yet
}

enum fdc_sectSize sectSizeForBytes(const uint_16 sectBytes) {
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

size_t bytesForSectSize(const enum fdc_sectSize sectSize) {
    return (sectSize == SECT_UNKNOWN) ? 0 : (128 << sectSize);
}

struct gnwStorGeometry driveGeometry(const uint_8 index) {
    struct gnwStorGeometry geometry = {0};

    // 3.5" 1.44 MB floppies supported only
    geometry.chs.c = 80;
    geometry.chs.h = 2;
    geometry.chs.s = 18;
    geometry.lba = chs2lba(geometry.chs.c, geometry.chs.h, geometry.chs.s, geometry.chs.h, geometry.chs.s);
    if (!geometry.lba) {
        memzero(&geometry, sizeof(struct gnwStorGeometry));
        return geometry;
    }
    geometry.sectSizeBytes = 512;

    return geometry;
}

uint_8 waitForInterrupt(const time_t ms) {
    for (time_t i = 0; i < ms; ++i) {
        if (irqRecv) { 
            irqRecv = 0; 
            return 1; 
        }

        extern void fdc_sleepms(const size_t);
        fdc_sleepms(1);
    }

    return 0;
}

bool isReadyForNonDMARead(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_RQM, msr) && IS_SET(BIT_MSR_DIO, msr) && IS_SET(BIT_MSR_NDMA, msr);
}

bool inNonDMAExecutionPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_NDMA, msr);
}

bool inPollingMode(const uint_16 base) {
    uint_8 sr0, pcn;
    proc_senseInterruptStatus(base, &sr0, &pcn);

    if ((sr0 & RANGE_SR0_IC) == RANGE_SR0_IC) {
        return true;
    }

    return false;
}

bool inCommandPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);

    return IS_SET(BIT_MSR_RQM, msr) && IS_NOT_SET(BIT_MSR_DIO, msr);
}

bool inResultPhase(const uint_16 base) {
    uint_8 msr = rdb(base + REG_MSR);
    
    return IS_SET(BIT_MSR_RQM, msr) && IS_SET(BIT_MSR_DIO, msr);
}

bool waitForCommandPhase(const uint_16 base) {
    for (uint_32 i = 0; i < FDC_COMMAND_PHASE_RETRY_COUNT; ++i) {
        if (inCommandPhase(base)) {
            return true;
        }

        extern void fdc_sleepms(const size_t);
        fdc_sleepms(FDC_PHASE_SLEEPMS_INTERVAL);
    }

    return false;
}

bool waitForResultPhase(const uint_16 base) {
    for (uint_32 i = 0; i < FDC_RESULT_PHASE_RETRY_COUNT; ++i) {
        if (inResultPhase(base)) {
            return true;
        }
    
        extern void fdc_sleepms(const size_t);
        fdc_sleepms(FDC_PHASE_SLEEPMS_INTERVAL);
    }

    return false;
}