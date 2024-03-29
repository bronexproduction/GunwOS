//
//  cmd_transfer.c
//  GunwOS
//
//  Created by Artur Danielewski on 26.03.2023.
//
//  Docs:
//  Intel 82077AA Datasheet 
//  5.1 - Data transfer commands
//

#include "cmd.h"
#include <utils.h>
#include "../common/io.h"

enum fdc_opStatus cmd_read(const uint_16 base, 
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
    TRY(pushData(base, (mode |
                       ((mode != RM_TRACK) ? (mt & BIT_MT) : 0x00) |
                       (mfm & BIT_MFM) |
                       ((mode != RM_TRACK) ? (sk & BIT_SK) : 0x00))));
    TRY(pushData(base, ((h & BIT_HDS) |
                       BIT_RANGE_ALIGNED(drive, RANGE_DS))));
    TRY(pushData(base, BIT_RANGE_ALIGNED(c, RANGE_C)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(h, RANGE_H)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(r, RANGE_R)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(n, RANGE_N)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(eot, RANGE_EOT)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(gpl, RANGE_GPL)));
    TRY(pushData(base, BIT_RANGE_ALIGNED(dtl, RANGE_DTL)));
    
    return OPSTATUS_OK;
}
