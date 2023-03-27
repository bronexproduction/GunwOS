//
//  proc.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//

#include "proc.h"
#include <error/panic.h>
#include <gunwctrl.h>
#include <driver/gunwbus.h>
#include "../common/io.h"
#include "../common/data.h"
#include "../common/strings.h"
#include "../utl/utl.h"
#include "../cmd/cmd.h"

static enum fdc_opStatus specify(const struct fdc_fddConfig config) {
    if (!config.base) {
        return OPSTATUS_INVPARAM;
    }

    return cmd_specify(config.base,
                       config.perf.srt,
                       config.perf.hut,
                       config.perf.hlt,
                       DMA_DISABLE);
}

/*
    Controller reset
    
    This code represents procedure described in
    8.2 Initialization
    Intel 82077AA Datasheet

    Flowchart present in the documentation is a bit misleading
    so this code represents a slightly modified version
    of flow described in the datasheet 
*/

enum fdc_opStatus proc_reset(const uint_16 base) {

    irqRecv = 0;
    
    pushReg(base, REG_DOR, CLEAR(BIT_DOR_RESET));
    pushReg(base, REG_DOR, SET(BIT_DOR_RESET));

    if (!waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {                          // WAIT FOR INTERRUPT
        proc_error(base, OPSTATUS_INTEXP, FDC_ERR_INIT_IRQ);
        return OPSTATUS_INTEXP;
    }

    enum fdc_opStatus status;
    if (inPollingMode(base)) {                                                  // IF POLLING MODE ENABLED
        for (int i = 0; i < FDC_FDD_PER_CONTROLLER-1; ++i) {                    // 3 TIMES (first has been done by inPollingMode):
            uint_8 data;
            status = proc_senseInterruptStatus(base, &data, &data);         // ISSUE SENSE INTERRUPT STATUS COMMAND
            if (status != OPSTATUS_OK) {
                proc_error(base, status, FDC_ERR_INIT_SIS);
                return status;
            }
        }
    }
    
    pushReg(base, REG_CCR, RATE_500KBPS);                                       // PROGRAM DATA RATE VIA CCR

    status = cmd_configure(base,                                            // ISSUE CONFIGURE COMMAND
                           ISEEK_ENABLE,
                           FIFO_ENABLE,
                           POLL_DISABLE,
                           FDC_FIFOTHR_DEFAULT,
                           FDC_PRECOMP_DEFAULT);

    if (status == OPSTATUS_NOT_READY) {   
        proc_error(base, status, FDC_ERR_INIT_CFG);
        return status;
    }
    else if (status == OPSTATUS_INVPARAM) {
        proc_error(base, status, FDC_ERR_INIT_CFG_PARAM);
        return status;
    }
        
    status = cmd_specify(base,
                         FDC_SRT_DEFAULT,
                         FDC_HUT_DEFAULT,
                         FDC_HLT_DEFAULT,
                         DMA_DISABLE);                                      // ISSUE SPECIFY COMMAND

    if (status != OPSTATUS_OK) {                        
        proc_error(base, status, FDC_ERR_INIT_SPC);
        return status;
    }

    for (uint_8 i = 0; i < FDC_FDD_SUPPORT_COUNT; ++i) {                        // FOR EACH DRIVE:
        proc_recalibrate(base, i);                                          // RECALIBRATE (dumb)
        proc_stopMotor(base, i);
    }

    return OPSTATUS_OK;                                                         // CONTROLLER READY
}

enum fdc_opStatus proc_recalibrate(const uint_16 base, const uint_8 drive) {
    
    enum fdc_opStatus s = proc_startMotor(base, drive);
    if (s != OPSTATUS_OK) return s;

    irqRecv = 0;

    s = cmd_recalibrate(base, drive);
    if (s != OPSTATUS_OK) {
        proc_stopMotor(base, drive);
        return s;
    }

    if (!waitForInterrupt(FDC_IRQDELAY_DEFAULT)) {
        proc_stopMotor(base, drive);
        OOPS("recalibrate: wait for interrupt failed");
        return OPSTATUS_INTEXP;
    }

    uint_8 sr0, pcn;
    proc_senseInterruptStatus(base, &sr0, &pcn);

    proc_stopMotor(base, drive);

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

void proc_error(const uint_16 base, enum fdc_opStatus s, const char * const reason) {
    #warning not implemented
    // s_trm_puts(reason);
    // s_trm_putc('\n');
}

enum fdc_opStatus proc_fdcDetect(const uint_16 base, const uint_8 drive, uint_8 * const present) {
    if (!present) return OPSTATUS_INVPARAM;
    if (drive >= FDC_FDD_SUPPORT_COUNT) return OPSTATUS_INVPARAM;
    
    enum fdc_opStatus s;
    s = proc_recalibrate(base, drive);
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

enum fdc_opStatus proc_prepare(const struct fdc_fddConfig config) {
    enum fdc_opStatus s = proc_startMotor(config.base, config.drive);
    if (s != OPSTATUS_OK) return s;

    s = specify(config);
    if (s != OPSTATUS_OK) {
        proc_stopMotor(config.base, config.drive);
        return s;
    }

    return OPSTATUS_OK;
}

enum fdc_opStatus proc_startMotor(const uint_16 base, const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }

    pushReg(base, REG_DOR, ALIGNED(1 << drive, RANGE_DOR_MC) |
                           ALIGNED(drive, RANGE_DS)          |
                           SET(BIT_DOR_RESET)                |
                           CLEAR(BIT_DOR_DACK));

    extern void fdc_sleepms(const size_t);
    fdc_sleepms(300);

    return OPSTATUS_OK;
}

enum fdc_opStatus proc_stopMotor(const uint_16 base, const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }

    uint_8 dor = rdb(base + REG_DOR) & ~ALIGNED(1 << drive, RANGE_DOR_MC);
    pushReg(base, REG_DOR, dor);

    return OPSTATUS_OK;
}

enum fdc_opStatus proc_senseInterruptStatus(const uint_16 base, uint_8 * const sr0, uint_8 * const pcn) {
    if (!sr0) return OPSTATUS_INVPARAM;
    if (!pcn) return OPSTATUS_INVPARAM;

    enum fdc_opStatus s;

    s = cmd_senseInterruptStatus(base);             // ISSUE SENSE INTERRUPT STATUS COMMAND
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

    return OPSTATUS_OK;
}
