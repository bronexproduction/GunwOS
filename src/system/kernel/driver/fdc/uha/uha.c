//
//  uha.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#include "uha.h"
#include <gunwstor.h>
#include <mem.h>
#include <utils.h>
#include <error/panic.h>
#include "../proc/proc.h"
#include "../cmd/cmd.h"
#include "../utl/utl.h"
#include "../common/data.h"
#include "../common/io.h"

bool uha_drivePresent(const uint_8 index) {
    const enum driveMask driveMask = 1 << index;
    
    return driveCount & driveMask;
}

struct gnwStorGeometry uha_driveGeometry(const uint_8 index) {
    return driveGeometry(index);
}

static size_t readSector(const struct fdc_fddConfig config,
                         const enum fdc_sectSize sectSize,
                         const size_t lba,
                         uint_8 * const buffer,
                         struct gnwStorError * const error) {
    
    /*
        Validate parameters
    */

    if (!error || !buffer) return 0;

    if (sectSize == SECT_UNKNOWN) {
        error->code = GSEC_INVALID_FORMAT;
        error->internalCode = 0;
        return 0;
    }
    if (bytesForSectSize(sectSize) != config.format.sys.sectSizeBytes) {
        error->code = GSEC_INVALID_ARGUMENT;
        error->internalCode = 0;
        return 0;
    }
    if (lba >= config.format.sys.lba) {
        error->code = GSEC_INVALID_ARGUMENT;
        error->internalCode = 0;
        return 0;
    }

    struct gnwStorCHS addr = lba2chs(lba, config.format.sys.chs.h, config.format.sys.chs.s);
    
    irqRecv = 0;

    /*
        Issue read command
    */

    enum fdc_opStatus s = cmd_read(config.base,
                                   RM_DATA,
                                   config.mt,
                                   config.mfm,
                                   SKIP_ENABLE,
                                   config.drive,
                                   addr.c,
                                   addr.h,
                                   addr.s,
                                   sectSize,
                                   config.format.sys.chs.s,
                                   config.format.gpl,
                                   0xFF);
    if (s != OPSTATUS_OK) {
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        OOPS("readSector: read failed", 0);
    }

    size_t currentBytes = 0;
    size_t demandedBytes = config.format.sys.sectSizeBytes;

    /*
        Wait for command result
    */

    do {
        if (!waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = OPSTATUS_INTEXP;
            OOPS("readSector: wait for interrupt failed", 0);
        }

        /*
            Read the FIFO
        */

        while (isReadyForNonDMARead(config.base)) {
            uint_8 dummyBuffer;
            uint_8 *dstBuffer;
            if (currentBytes >= demandedBytes) {
                /* 
                    Multi-Sector Read Operation
                    may read more sectors than required,
                    but FIFO has to be flushed anyway
                */
                dstBuffer = &dummyBuffer;
            } else {
                dstBuffer = buffer + currentBytes;
            }
            
            s = pullData(config.base, dstBuffer);
            if (s != OPSTATUS_OK) {
                error->code = GSEC_COMMAND_FAILED;
                error->internalCode = s;
                OOPS("readSector: pull data failed", 0);
            }

            ++currentBytes;
        }
    } while (inNonDMAExecutionPhase(config.base));

    uint_8 sr0;
    s = pullData(config.base, &sr0);
    if (s != OPSTATUS_OK) {
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        OOPS("readSector: pull sr0 failed", 0);
    }
    uint_8 sr1;
    s = pullData(config.base, &sr1);
    if (s != OPSTATUS_OK) {
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        OOPS("readSector: pull sr1 failed", 0);
    }

    switch (sr0 & RANGE_SR0_IC) {
    case 0x00:
        /*
            Normal termination
        */
        error->code = GSEC_NONE;
        error->internalCode = 0;
        break;
    case 0x40:
        /*
            Abnormal termination
        */
        switch (sr1) {
        case BIT_SR1_EN:
            error->code = GSEC_MEDIA_NOT_PRESENT;
            error->internalCode = OPSTATUS_END_OF_CYLINDER;
            break;
        default:
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = OPSTATUS_ABNORMAL_TERM;
            OOPS("readSector: IC failed", 0);
            break;
        }
        break;
    default:
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = OPSTATUS_ABNORMAL_TERM;
        OOPS("readSector: IC failed", 0);
        break;
    }

    uint_8 data;
    for (int i = 0; i < 5; ++i) {
        // read ST 2, C, H, R, N
        s = pullData(config.base, &data);
        if (s != OPSTATUS_OK) {
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = s;
            OOPS("readSector: pull register failed", 0);
        }
    }

    return (error->code == GSEC_NONE) ? MIN(currentBytes, demandedBytes) : 0;
}

size_t uha_read(const uint_8 index,
                const size_t lba,
                const size_t count,
                uint_8 * const buffer,
                struct gnwStorError * const error) {

    /*
        Validate parameters
    */

    if (!error || !buffer) return 0;

    const struct fdc_fddConfig config = configFor(index);
    const enum fdc_sectSize sectSize = sectSizeForBytes(config.format.sys.sectSizeBytes);

    if (sectSize == SECT_UNKNOWN) {
        error->code = GSEC_INVALID_FORMAT;
        error->internalCode = 0;
        return 0;
    }

    /*
        Start the motor and specify drive reliability parameters
    */

    enum fdc_opStatus s = proc_prepare(config);
    if (s != OPSTATUS_OK) {        
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        OOPS("fdc_uha_read: prepare failed", 0);
    }

    /*
        Read bytes
    */

    size_t totalBytes = 0;
    for (size_t currentSector = 0; currentSector < count; ++currentSector) {
        uint_8 localBuffer[config.format.sys.sectSizeBytes];

        size_t bytes = readSector(config, sectSize, lba + currentSector, localBuffer, error);
        if (error->code) {
            proc_stopMotor(config.base, config.drive);
            return 0;
        }
        if (bytes != config.format.sys.sectSizeBytes) {
            proc_stopMotor(config.base, config.drive);
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = 0;
            return 0;
        }

        memcopy(localBuffer, buffer + (currentSector * config.format.sys.sectSizeBytes), config.format.sys.sectSizeBytes);
        totalBytes += bytes;
    }

    /*
        Stop motor and return byte count
    */

    proc_stopMotor(config.base, config.drive);
    
    error->code = 0;
    error->internalCode = 0;

    return totalBytes;
}
