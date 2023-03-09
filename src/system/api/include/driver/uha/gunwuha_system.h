//
//  gunwuha_system.h
//  Universal Hardware Abstraction for system device
//  GunwOS
//
//  Created by Artur Danielewski on 22.01.2021.
//

#ifndef GUNWOS_GUNWUHA_SYSTEM_H
#define GUNWOS_GUNWUHA_SYSTEM_H

#include <uha/gunwuha_system_desc.h>

struct gnwDeviceUHA_system_routine {
    void (*_unused)();
};

struct gnwDeviceUHA_system {
    struct gnwDeviceUHA_system_desc desc;
    struct gnwDeviceUHA_system_routine routine;
};

#endif // GUNWOS_GUNWUHA_SYSTEM_H
