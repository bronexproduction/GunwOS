//
//  gunwdriver.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwdrv.h"

extern const struct gnwDeviceDescriptor _gnw_device_descriptor;

void _require_external_dependencies() {
    const void * device_descriptor_unused = &_gnw_device_descriptor; (void)device_descriptor_unused;
}

#endif // _GUNWAPI_KERNEL
