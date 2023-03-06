//
//  dev.h
//  GunwOS
//
//  Created by Artur Danielewski on 06.03.2023.
//

#ifndef DEV_H
#define DEV_H

#include <gunwdev.h>

enum gnwDriverError k_dev_install(size_t * const id, const struct gnwDeviceDescriptor * const descriptor);
enum gnwDriverError k_dev_start(size_t id);

#endif // DEV_H
