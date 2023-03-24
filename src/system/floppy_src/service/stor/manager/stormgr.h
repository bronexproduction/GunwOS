//
//  stormgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 02.04.2020.
//

#include "../stor.h"
#include "../../../../include/gunwstor.h"

struct s_stor_drvDescriptor s_stormgr_drive(const uint_32 driveID);
struct s_stor_volDescriptor s_stormgr_volume(const uint_32 volumeID);

size_t s_stormgr_drvRead(const uint_32 volumeID,
                         const size_t lba,
                         uint_8 *buffer,
                         const size_t byteCount,
                         struct gnwStorError *error);

size_t s_stormgr_drvWrite(const uint_32 volumeID,
                          const size_t lba,
                          const uint_8 * const buffer,
                          const size_t byteCount,
                          struct gnwStorError *error);
