//
//  _gunwmem.h
//  GunwOS
//
//  Created by Artur Danielewski on 05.11.2024.
//

#ifndef GUNWOS__GUNWMEM_H
#define GUNWOS__GUNWMEM_H

#include <defs.h>
#include <types.h>
#include "../include/gunwmemtypes.h"

#define PAGE_SIZE KiB(4)

bool isHeapUsed();

#endif // GUNWOS__GUNWMEM_H
