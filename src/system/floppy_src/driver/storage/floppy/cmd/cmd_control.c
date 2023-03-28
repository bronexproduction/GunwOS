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

enum k_fdc_opStatus fdc_cmd_seek(const uint_16 base,
                                 const enum fdc_seekMode mode) {
    
    // TODO
    
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

enum k_fdc_opStatus fdc_cmd_dumpreg(const uint_16 base) {
    TRY(pushData(base, 0x0E))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_perpendicularMode(const uint_16 base) {
    
    // TODO
    
    return OPSTATUS_OK;
}
