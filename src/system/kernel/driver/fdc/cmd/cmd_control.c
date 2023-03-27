//
//  cmd_control.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//  5.2 - Control commands 
//

#include "cmd.h"
#include "../common/defaults.h"
#include "../common/io.h"

enum fdc_opStatus cmd_specify(const uint_16 base, 
                              const uint_8 srt,
                              const uint_8 hut,
                              const uint_8 hlt,
                              const enum fdc_dmaMode dmaMode) {
    TRY(pushData(base, 0x03));
    TRY(pushData(base, ALIGNED(srt, RANGE_SRT) |
                       ALIGNED(hut, RANGE_HUT)));
    TRY(pushData(base, ALIGNED(hlt, RANGE_HLT) |
                       (dmaMode & BIT_ND)));
    
    return OPSTATUS_OK;
}

enum fdc_opStatus cmd_recalibrate(const uint_16 base, 
                                  const uint_8 drive) {
    if (drive >= FDC_FDD_SUPPORT_COUNT) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, 0x07));
    TRY(pushData(base, ALIGNED(drive, RANGE_DS)));

    return OPSTATUS_OK;
}

enum fdc_opStatus cmd_senseInterruptStatus(const uint_16 base) {
    TRY(pushData(base, 0x08));
    
    return OPSTATUS_OK;
}

enum fdc_opStatus cmd_version(const uint_16 base) {
    TRY(pushData(base, 0x10));
    
    return OPSTATUS_OK;
}

enum fdc_opStatus cmd_configure(const uint_16 base,
                                const enum fdc_eis eis,
                                const enum fdc_efifo efifo,
                                const enum fdc_poll poll,
                                const uint_8 fifothr,
                                const uint_8 pretrk) {
    if (fifothr > 0x0F) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, 0x13));
    TRY(pushData(base, 0x00));
    TRY(pushData(base, (eis     & BIT_EIS      ) |
                       (efifo   & BIT_EFIFO    ) |
                       (poll    & BIT_POLL     ) |
                       (fifothr & RANGE_FIFOTHR)));
    TRY(pushData(base, pretrk & RANGE_PRETRK));
    
    return OPSTATUS_OK;
}