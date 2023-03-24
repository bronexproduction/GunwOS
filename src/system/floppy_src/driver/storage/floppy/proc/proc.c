//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 29.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#include "proc.h"

#include "../cmd/cmd.h"
#include "../shared/io.h"
#include "../shared/defaults.h"
#include "../../../../kernel/timer/timer.h"

static enum k_fdc_opStatus specify(const struct fdc_fddConfig config);

void fdc_proc_error(const uint_16 base, enum k_fdc_opStatus s, const char * const reason) {
    #warning not implemented
    s_trm_puts(reason);
    s_trm_putc('\n');
}

enum k_fdc_opStatus fdc_proc_fdcDetect(const uint_16 base, const uint_8 drive, uint_8 * const present) {
    if (!present) return OPSTATUS_INVPARAM;
    if (drive >= FDC_FDD_SUPPORT_COUNT) return OPSTATUS_INVPARAM;
    
    enum k_fdc_opStatus s;
    s = fdc_proc_recalibrate(base, drive);
    if (s == OPSTATUS_NOT_READY) {
        return OPSTATUS_NOT_READY;
    }
    else if (s == OPSTATUS_INVDRIVE) {
        *present = 0;
    }
    else if (s == OPSTATUS_OK) {
        *present = 1;
    }
    else {
        *present = 0;
    }

    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_proc_recalibrate(const uint_16 base, const uint_8 drive) {
    
    enum k_fdc_opStatus s = fdc_proc_startMotor(base, drive);
    if (s != OPSTATUS_OK) return s;

    extern uint_8 fdc_irqRecv;
    fdc_irqRecv = 0;

    s = fdc_cmd_recalibrate(base, drive);
    if (s != OPSTATUS_OK) {
        fdc_proc_stopMotor(base, drive);
        return s;
    }

    extern uint_8 fdc_waitForInterrupt(const time_t);
    if (!fdc_waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {
        s_trm_puts("recalibrate: wait for interrupt failed\n");
        fdc_proc_stopMotor(base, drive);
        return OPSTATUS_INTEXP;
    }

    uint_8 sr0, pcn;
    fdc_proc_senseInterruptStatus(base, &sr0, &pcn);

    fdc_proc_stopMotor(base, drive);

    uint_8 ic = sr0 & RANGE_SR0_IC;
    uint_8 se = sr0 & BIT_SR0_SE;
    uint_8 ec = sr0 & BIT_SR0_EC;
    uint_8 ds = sr0 & RANGE_DS;

    if (ds != drive) {                                      // Received data for other drive than expected
        return OPSTATUS_INVDRIVE;                           // Controller error trying to access drive - assume invalid
    }
    if (ic == 0x80) {
        return OPSTATUS_INVCMD;                             // Invalid command
    }
    if (ic == 0x40) {
        return OPSTATUS_NOTFIN;                             // Abnormal termination - not started or not completed
    }
    if (!se) {
        return OPSTATUS_INVCMD;                             // FDC did not complete expected command
    }
    if (ec) {
        return OPSTATUS_FAIL;                               // if EC flag on - recalibrate failed
    }

    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_proc_senseInterruptStatus(const uint_16 base, uint_8 * const sr0, uint_8 * const pcn) {
    if (!sr0) return OPSTATUS_INVPARAM;
    if (!pcn) return OPSTATUS_INVPARAM;

    enum k_fdc_opStatus s;

    s = fdc_cmd_senseInterruptStatus(base);         // ISSUE SENSE INTERRUPT STATUS COMMAND
    if (s != OPSTATUS_OK) {
        return s;
    }
    
    s = pullData(base, sr0);                        // READ SR0
    if (s != OPSTATUS_OK) {
        return s;
    }
    
    s = pullData(base, pcn);                        // READ PCN
    if (s != OPSTATUS_OK) {
        return s;
    }
}

enum k_fdc_opStatus fdc_proc_prepare(const struct fdc_fddConfig config) {
    enum k_fdc_opStatus s = fdc_proc_startMotor(config.base, config.drive);
    if (s != OPSTATUS_OK) return s;

    s = specify(config);
    if (s != OPSTATUS_OK) {
        fdc_proc_stopMotor(config.base, config.drive);
        return s;
    }

    return OPSTATUS_OK;
}

static enum k_fdc_opStatus specify(const struct fdc_fddConfig config) {
    if (!config.base) {
        return OPSTATUS_INVPARAM;
    }

    return fdc_cmd_specify(config.base,
                           config.perf.srt,
                           config.perf.hut,
                           config.perf.hlt,
                           DMA_DISABLE);
}

enum k_fdc_opStatus fdc_proc_startMotor(const uint_16 base, const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }

    pushReg(base, REG_DOR, ALIGNED(1 << drive, RANGE_DOR_MC) |
                           ALIGNED(drive, RANGE_DS)          |
                           SET(BIT_DOR_RESET)                |
                           CLEAR(BIT_DOR_DACK));

    sleepms(300);

    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_proc_stopMotor(const uint_16 base, const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }

    uint_8 dor = rdb(base + REG_DOR) & ~ALIGNED(1 << drive, RANGE_DOR_MC);
    pushReg(base, REG_DOR, dor);

    return OPSTATUS_OK;
}
