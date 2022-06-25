//
//  devmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//

#include "../../../../lib/stdgunw/types.h"
#include "../../../include/gunwdrv.h"

/*
    Returns number of available device descriptors
*/
size_t s_devmgr_descriptorCount();

/*
    Listing of devices of given type
*/
struct gnwDeviceDescriptor s_devmgr_descriptorFor(const uint_32 descriptorID);

/*
    Installs new device and the driver
*/
enum gnwDriverError s_devmgr_install(struct gnwDeviceDescriptor descriptor);
