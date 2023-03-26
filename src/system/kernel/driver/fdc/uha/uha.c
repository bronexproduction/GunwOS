//
//  uha.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//

#include "uha.h"
#include <gunwstor.h>
#include <error/panic.h>
#include "../proc/proc.h"
#include "../cmd/cmd.h"
#include "../utl/utl.h"
#include "../common/defaults.h"
#include "../common/io.h"

bool uha_drivePresent(const uint_8 index) {
    const enum driveMask driveMask = 1 << index;
    
    extern uint_8 fdc_drives;
    return fdc_drives & driveMask;
}

struct gnwStorGeometry uha_driveGeometry(const uint_8 index) {
    return driveGeometry(index);
}

static size_t readSector(const struct fdc_fddConfig config,
                         const enum fdc_sectSize sectSize,
                         const size_t lba,
                         const size_t count,
                         uint_8 * const buffer,
                         struct gnwStorError * const error) {
    
    if (!error || !buffer) return 0;

    if (sectSize == SECT_UNKNOWN) {
        error->code = GSEC_INVALID_FORMAT;
        error->internalCode = 0;
        return 0;
    }

    struct gnwStorCHS addr = lba2chs(lba, config.format.sys.chs.h, config.format.sys.chs.s);
    
    extern uint_8 fdc_irqRecv;
    fdc_irqRecv = 0;

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
        OOPS("readSector: read failed");
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        return 0;
    }

    size_t currentBytes = 0;

    do {
        if (!waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {
            OOPS("readSector: wait for interrupt failed");
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = OPSTATUS_INTEXP;

            return 0;
        }

        while (isReadyForNonDMARead(config.base)) {
            OOPS("Ready for non-DMA read");
            if (currentBytes >= sectSize) {
                OOPS("readSector: buffer overflow");
                error->code = GSEC_BUFFER_OVERFLOW;
                error->internalCode = 0;

                return 0;
            }

            s = pullData(config.base, buffer + currentBytes);
            if (s != OPSTATUS_OK) {
                OOPS("readSector: pull data failed");
                error->code = GSEC_COMMAND_FAILED;
                error->internalCode = s;

                return 0;
            }

            ++currentBytes;
        }
    } while (fdc_inNonDMAExecutionPhase(config.base));
    
    uint_8 sr0;
    s = pullData(config.base, &sr0);
    if (s != OPSTATUS_OK) {
        OOPS("readSector: pull sr0 failed");
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;

        return 0;
    }

    if ((sr0 & RANGE_SR0_IC) == 0x40) {
        OOPS("readSector: IC failed");
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = OPSTATUS_ABNORMAL_TERM;

        return 0;
    }

    uint_8 data;
    
    for (int i = 0; i < 6; ++i) {
        // read ST 1, ST 2, C, H, R, N
        s = pullData(config.base, &data);
        if (s != OPSTATUS_OK) {
            OOPS("readSector: pull register failed");
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = s;

            return 0;
        }
    }

    error->code = 0;
    error->internalCode = 0;

    return currentBytes;
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
    const enum fdc_sectSize sectSize = sectSizeFor(config.format.sys.sectSize);
    
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
        OOPS("fdc_uha_read: prepare failed");
        error->code = GSEC_COMMAND_FAILED;
        error->internalCode = s;
        return 0;
    }

    /*
        Read bytes
    */

    size_t totalBytes = 0;
    for (size_t currentSector = 0; currentSector < count; ++currentSector) {
        uint_8 localBuffer[sectSize];
        size_t bytes = readSector(config, sectSize, lba, count, localBuffer, error);
        if (error->code) {
            proc_stopMotor(config.base, config.drive);
            return 0;
        }
        if (bytes != sectSize) {
            proc_stopMotor(config.base, config.drive);
            error->code = GSEC_COMMAND_FAILED;
            error->internalCode = 0;
            return 0;
        }

        memcopy(localBuffer, buffer + (currentSector * sectSize), sectSize);
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
