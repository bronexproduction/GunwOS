//
//  volume.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.03.2023.
//

#ifndef VOLUME_H
#define VOLUME_H

#include <storage/storage.h>

enum k_stor_error k_stor_vol_readHeader(const size_t volumeId, uint_8 * const header);

#endif // VOLUME_H
