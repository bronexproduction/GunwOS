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
#include "../common/defaults.h"

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

    extern uint_8 irqRecv;
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
