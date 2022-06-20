//
//  floppy.h
//  GunwOS
//
//  Created by Artur Danielewski on 02.04.2020.
//

#include "shared/types.h"
                  
size_t k_fdc_write(const uint_32 driveID,
                   const size_t lba,
                   const char * const buffer,
                   const size_t bufferSize,
                   enum k_fdc_opStatus *status);

