//
//  devmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2020.
//

#include <stdgunw/types.h>
#include <gunwdrv.h>

/*
    Returns number of available device descriptors
*/
size_t k_devmgr_descriptorCount();

/*
    Listing of devices of given type
*/
struct gnwDeviceDescriptor k_devmgr_descriptorFor(const uint_32 descriptorID);

/*
    Installs new device and the driver
*/
enum gnwDriverError k_devmgr_install(struct gnwDeviceDescriptor descriptor);
