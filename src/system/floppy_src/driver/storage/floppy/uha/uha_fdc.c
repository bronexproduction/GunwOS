//
//  uha_fdc.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.01.2021.
//

#include "../../../../../../lib/stdgunw/types.h"
#include "../../../../../../lib/stdgunw/mem.h"
#include "../floppy.h"
#include "../shared/io.h"
#include "../../../../../include/gunwstor.h"
#include "../shared/defaults.h"
#include "../proc/proc.h"

// TODO: remove
extern int s_trm_puts(const char * const s);

uint_8 s_drv_fdc_uha_drivePresent(const uint_8 index) {
    const enum driveMask driveMask = 1 << index;
    
    extern uint_8 fdc_drives;
    return fdc_drives & driveMask;
}

struct gnwStorGeometry s_drv_fdc_uha_driveGeometry(const uint_8 index) {
    struct gnwStorGeometry geometry;

    // 3.5" 1.44 MB floppies supported only
    geometry.chs.c = 80;
    geometry.chs.h = 2;
    geometry.chs.s = 18;
    geometry.lba = k_utl_chs2lba(geometry.chs.c, geometry.chs.h, geometry.chs.s, geometry.chs.h, geometry.chs.s);
    geometry.sectSize = 512;

    return geometry;
}

extern struct fdc_fddConfig fdc_configFor(const uint_32 driveID);
extern uint_8 fdc_waitForInterrupt(const time_t);
extern uint_8 fdc_readyForNonDMARead(const uint_16 base);
extern uint_8 fdc_inNonDMAExecutionPhase(const uint_16 base);
extern uint_8 fdc_waitForNonDMAExecutionPhase(const uint_16 base);

static size_t readSector(const struct fdc_fddConfig config,
                         const enum fdc_sectSize sectSize,
                         const size_t lba,
                         const size_t count,
                         uint_8 * const buffer,
                         struct gnwStorError * const error);

size_t s_drv_fdc_uha_read(const uint_8 index,
                          const size_t lba,
                          const size_t count,
                          uint_8 * const buffer,
                          struct gnwStorError * const error) {

    /*
        Validate parameters
    */

    if (!error || !buffer) return 0;

    const struct fdc_fddConfig config = fdc_configFor(index);
    const enum fdc_sectSize sectSize = fdc_sectSizeFor(config.format.sys.sectSize);
    
    if (sectSize == SECT_UNKNOWN) {
        error->code = INVALID_FORMAT;
        error->internalCode = 0;
        return 0;
    }

    /*
        Start the motor and specify drive reliability parameters
    */

    enum k_fdc_opStatus s = fdc_proc_prepare(config);
    if (s != OPSTATUS_OK) {
        s_trm_puts("s_drv_fdc_uha_read: prepare failed\n");
        error->code = COMMAND_FAILED;
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
            fdc_proc_stopMotor(config.base, config.drive);
            return 0;
        }
        if (bytes != sectSize) {
            fdc_proc_stopMotor(config.base, config.drive);
            error->code = COMMAND_FAILED;
            error->internalCode = 0;
            return 0;
        }

        memcopy(localBuffer, buffer + (currentSector * sectSize), sectSize);
        totalBytes += bytes;
    }

    /*
        Stop motor and return byte count
    */

    fdc_proc_stopMotor(config.base, config.drive);
    error->code = 0;
    error->internalCode = 0;
    return totalBytes;
}

static size_t readSector(const struct fdc_fddConfig config,
                         const enum fdc_sectSize sectSize,
                         const size_t lba,
                         const size_t count,
                         uint_8 * const buffer,
                         struct gnwStorError * const error) {
    
    if (!error || !buffer) return 0;

    if (sectSize == SECT_UNKNOWN) {
        error->code = INVALID_FORMAT;
        error->internalCode = 0;
        return 0;
    }

    struct gnwStorCHS addr = k_utl_lba2chs(lba, config.format.sys.chs.h, config.format.sys.chs.s);
    
    extern uint_8 fdc_irqRecv;
    fdc_irqRecv = 0;

    enum k_fdc_opStatus s = fdc_cmd_read(config.base,
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
        s_trm_puts("readSector: read failed\n");
        error->code = COMMAND_FAILED;
        error->internalCode = s;
        return 0;
    }

    size_t currentBytes = 0;

    do {
        if (!fdc_waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {
            s_trm_puts("readSector: wait for interrupt failed\n");
            error->code = COMMAND_FAILED;
            error->internalCode = OPSTATUS_INTEXP;

            return 0;
        }

        while (fdc_readyForNonDMARead(config.base)) {
            s_trm_puts("Ready for non-DMA read\n\0");
            if (currentBytes >= sectSize) {
                s_trm_puts("readSector: buffer overflow\n");
                error->code = BUFFER_OVERFLOW;
                error->internalCode = 0;

                return 0;
            }

            s = pullData(config.base, buffer + currentBytes);
            if (s != OPSTATUS_OK) {
                s_trm_puts("readSector: pull data failed\n");
                error->code = COMMAND_FAILED;
                error->internalCode = s;

                return 0;
            }

            ++currentBytes;
            s_trm_puts("Byte read\n\0");
        }
    } while (fdc_inNonDMAExecutionPhase(config.base));
    
    uint_8 sr0;
    s = pullData(config.base, &sr0);
    if (s != OPSTATUS_OK) {
        s_trm_puts("readSector: pull sr0 failed\n");
        error->code = COMMAND_FAILED;
        error->internalCode = s;

        return 0;
    }

    if ((sr0 & RANGE_SR0_IC) == 0x40) {
        s_trm_puts("readSector: IC failed\n");
        error->code = COMMAND_FAILED;
        error->internalCode = OPSTATUS_ABNORMAL_TERM;

        return 0;
    }

    uint_8 data;
    
    for (int i = 0; i < 6; ++i) {
        s_trm_putin(i);
        // read ST 1, ST 2, C, H, R, N
        s = pullData(config.base, &data);
        if (s != OPSTATUS_OK) {
            s_trm_puts("readSector: pull register failed\n");
            error->code = COMMAND_FAILED;
            error->internalCode = s;

            return 0;
        }

        s_trm_puts(":\0");
        s_trm_putun_h(data);
        s_trm_puts(" \0");
    }

    error->code = 0;
    error->internalCode = 0;

    return currentBytes;
}
