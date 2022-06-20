//
//  cmd_control.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//  5.2 - Control commands 
//

#include "../shared/io.h"
#include "../shared/defaults.h"
#include "../proc/proc.h"

enum k_fdc_opStatus fdc_cmd_readID(const uint_16 base,
                                   const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }
    
    // TODO

    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_recalibrate(const uint_16 base, 
                                        const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, 0x07))
    TRY(pushData(base, ALIGNED(drive, RANGE_DS)))

    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_seek(const uint_16 base,
                                 const enum fdc_seekMode mode) {
    
    // TODO
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_senseInterruptStatus(const uint_16 base) {
    TRY(pushData(base, 0x08))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_senseDriveStatus(const uint_16 base,
                                             const uint_8 hds,
                                             const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, 0x04))
    TRY(pushData(base, hds & BIT_HDS |
                       ALIGNED(drive, RANGE_DS)))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_specify(const uint_16 base, 
                                    const uint_8 srt,
                                    const uint_8 hut,
                                    const uint_8 hlt,
                                    const enum fdc_dmaMode dmaMode) {
    TRY(pushData(base, 0x03))
    TRY(pushData(base, ALIGNED(srt, RANGE_SRT) | 
                       ALIGNED(hut, RANGE_HUT)))
    TRY(pushData(base, ALIGNED(hlt, RANGE_HLT) |
                       dmaMode & BIT_ND))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_configure(const uint_16 base,
                                      const enum fdc_eis eis,
                                      const enum fdc_efifo efifo,
                                      const enum fdc_poll poll,
                                      const uint_8 fifothr,
                                      const uint_8 pretrk) {
    if (fifothr > 0x0F) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, 0x13))
    TRY(pushData(base, 0x00))
    TRY(pushData(base, eis     & BIT_EIS      |
                       efifo   & BIT_EFIFO    |
                       poll    & BIT_POLL     |
                       fifothr & RANGE_FIFOTHR))
    TRY(pushData(base, pretrk & RANGE_PRETRK))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_version(const uint_16 base) {
    TRY(pushData(base, 0x10))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_dumpreg(const uint_16 base) {
    TRY(pushData(base, 0x0E))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_perpendicularMode(const uint_16 base) {
    
    // TODO
    
    return OPSTATUS_OK;
}
