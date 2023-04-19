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
    /*
        Retrieves the value of the parameter with given description

        Params:
            * param - device-specific parameter code
            * subParam - additional parameter
            * paramIndex - index of the value
            * result - pointer to result storage location

        Return value: true on success, false otherwise

        The driver can implement custom API using that function.
        Some drivers are required to implement it.
        Check UHA for particular device for more details.
    */
    bool (*getParam)(const size_t param,
                     const size_t subParam,
                     const size_t paramIndex,
                     size_t * const result);
};

struct gnwDeviceUHA_system {
    struct gnwDeviceUHA_system_desc desc;
    struct gnwDeviceUHA_system_routine routine;
};

#endif // GUNWOS_GUNWUHA_SYSTEM_H
