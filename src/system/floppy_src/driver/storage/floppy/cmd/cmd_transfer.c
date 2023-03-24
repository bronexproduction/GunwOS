//
//  cmd_transfer.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2020.
//
//  Docs:
//  Intel 82077AA Datasheet 
//  5.1 - Data transfer commands
//

#include "../shared/io.h"
#include "../proc/proc.h"

enum k_fdc_opStatus fdc_cmd_read(const uint_16 base, 
                                 const enum fdc_readMode mode,
                                 const uint_8 mt,
                                 const uint_8 mfm,
                                 const uint_8 sk,
                                 const uint_8 drive,
                                 const uint_8 c,
                                 const uint_8 h, 
                                 const uint_8 r,
                                 const uint_8 n,
                                 const uint_8 eot,
                                 const uint_8 gpl,
                                 const uint_8 dtl) {

    #warning reading multiple sectors may be a lot more complicated

    if (drive > 0x03) {
        return OPSTATUS_INVPARAM;
    }
    extern int s_trm_puts(const char * const s);
    TRY(pushData(base, (mode |
                       ((mode != RM_TRACK) ? (mt & BIT_MT) : 0x00) |
                       (mfm & BIT_MFM) |
                       ((mode != RM_TRACK) ? (sk & BIT_SK) : 0x00))))
    TRY(pushData(base, (h & BIT_HDS |
                       ALIGNED(drive, RANGE_DS))))
    TRY(pushData(base, ALIGNED(c, RANGE_C)))
    TRY(pushData(base, ALIGNED(h, RANGE_H)))
    TRY(pushData(base, ALIGNED(r, RANGE_R)))
    TRY(pushData(base, ALIGNED(n, RANGE_N)))
    TRY(pushData(base, ALIGNED(eot, RANGE_EOT)))
    TRY(pushData(base, ALIGNED(gpl, RANGE_GPL)))
    TRY(pushData(base, ALIGNED(dtl, RANGE_DTL)))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_write(const uint_16 base,
                                  const enum fdc_writeMode mode,
                                  const uint_8 mt,
                                  const uint_8 mfm,
                                  const uint_8 hds,
                                  const uint_8 drive,
                                  const uint_8 c,
                                  const uint_8 h,
                                  const uint_8 r,
                                  const uint_8 n,
                                  const uint_8 eot,
                                  const uint_8 gpl,
                                  const uint_8 dtl) {
    if (drive > 0x03) {
        return OPSTATUS_INVPARAM;
    }
    TRY(pushData(base, mode |
                       mt & BIT_MT |
                       mfm & BIT_MFM))
    TRY(pushData(base, hds & BIT_HDS |
                       ALIGNED(drive, RANGE_DS)))
    TRY(pushData(base, ALIGNED(c, RANGE_C)))
    TRY(pushData(base, ALIGNED(h, RANGE_H)))
    TRY(pushData(base, ALIGNED(r, RANGE_R)))
    TRY(pushData(base, ALIGNED(n, RANGE_N)))
    TRY(pushData(base, ALIGNED(eot, RANGE_EOT)))
    TRY(pushData(base, ALIGNED(gpl, RANGE_GPL)))
    TRY(pushData(base, ALIGNED(dtl, RANGE_DTL)))
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_verify(const uint_16 base) {
    #warning Not implemented yet
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_formatTrack(const uint_16 base) {
    #warning Not implemented yet
    
    return OPSTATUS_OK;
}

enum k_fdc_opStatus fdc_cmd_scan(const uint_16 base,
                                 const enum fdc_scanMode mode) {
    #warning Not implemented yet
    
    return OPSTATUS_OK;
}
