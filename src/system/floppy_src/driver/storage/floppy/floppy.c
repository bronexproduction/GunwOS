//
//  floppy.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.04.2020.
//

#include "shared/types.h"

extern struct fdc_fddConfig fdc_configFor(const uint_32 driveID);
extern enum k_fdc_opStatus fdc_proc_prepare(const struct fdc_fddConfig config);

size_t k_fdc_write(const uint_32 driveID,
                   const size_t lba,
                   const char * const buffer,
                   const size_t bufferSize,
                   enum k_fdc_opStatus *status) {
    struct fdc_fddConfig config = fdc_configFor(driveID);
    struct gnwStorCHS addr = k_utl_lba2chs(lba, 0, 0);
    enum fdc_sectSize sectSize = fdc_sectSizeFor(config.format.sys.sectSize);
    
    if (sectSize == SECT_UNKNOWN) {
        if (status) *status = OPSTATUS_INVPARAM;
        return 0;
    }

    enum k_fdc_opStatus s = fdc_proc_prepare(config);
    if (s != OPSTATUS_OK) {
        if (status) *status = s;
        return 0;
    }
    
    fdc_cmd_write(config.base,
                  WM_DATA,
                  config.mt,
                  config.mfm,
                  addr.h,
                  config.drive,
                  addr.c,
                  addr.h,
                  addr.s,
                  sectSize,
                  config.format.sys.chs.s,
                  config.format.gpl,
                  0xFF);

    return 0;
    #warning not implemented yet
}
