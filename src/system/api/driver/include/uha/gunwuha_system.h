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
#include <gunwdevtypes.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_SYSTEM GNW_UHA_EMPTY(SYSTEM)
#define GNW_UHA_SYSTEM_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

typedef void (*gnwDeviceGetParamQueryDecoder)(const ptr_t, struct gnwDeviceGetParamQuery * const);
typedef void (*gnwDeviceSetParamQueryDecoder)(const ptr_t, struct gnwDeviceSetParamQuery * const);

struct gnwDeviceUHA_system_routine {
    /*
        Retrieves the value of the parameter with given description

        Params:
            * queryPtr - pointer to struct gnwDeviceGetParamQuery

        It's expected for the driver to call getParamReply on finish

        The driver can implement custom API using that function.
        Some drivers are required to implement it.
        Check UHA for particular device for more details.
    */
    void (*getParam)(const struct gnwDeviceGetParamQuery * const queryPtr);
    
    /*
        Decoder for gnwDeviceGetParamQuery objects

        Required if getParam defined
    */
    gnwDeviceGetParamQueryDecoder getParamDecoder;
    
    /*
        Sets the value of the parameter with given description

        Params:
            * queryPtr - pointer to struct gnwDeviceSetParamQuery

        Return value: true on success, false otherwise

        The driver can implement custom API using that function.
        Some drivers are required to implement it.
        Check UHA for particular device for more details.
    */
    bool (*setParam)(const size_t param,
                     const size_t subParam,
                     const size_t paramIndex,
                     const size_t value);
    
    /*
        Decoder for gnwDeviceSetParamQuery objects

        Required if setParam defined
    */
    gnwDeviceSetParamQueryDecoder setParamDecoder;
};

struct gnwDeviceUHA_system {
    struct gnwDeviceUHA_system_desc desc;
    struct gnwDeviceUHA_system_routine routine;
};

#endif // GUNWOS_GUNWUHA_SYSTEM_H
