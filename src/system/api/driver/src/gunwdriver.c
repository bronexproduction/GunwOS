//
//  gunwdriver.c
//  GunwOS
//
//  Created by Artur Danielewski on 17.11.2024.
//

#ifndef _GUNWAPI_KERNEL

#include "../include/gunwdrv.h"
#include "../include/gunwrlp.h"

extern const size_t _gnw_device_descriptor_count;
extern const struct gnwDeviceDescriptor _gnw_device_descriptor_list[];
extern const size_t _gnw_device_identifier_list[];

void _require_external_dependencies() {
    const void * device_descriptor_count_unused = &_gnw_device_descriptor_count; (void)device_descriptor_count_unused;
    const void * device_descriptor_list_unused = _gnw_device_descriptor_list; (void)device_descriptor_list_unused;
    const void * device_identifier_list_unused = _gnw_device_identifier_list; (void)device_identifier_list_unused;
}

void dupa() {
    runLoopStart();
}

#endif // _GUNWAPI_KERNEL
