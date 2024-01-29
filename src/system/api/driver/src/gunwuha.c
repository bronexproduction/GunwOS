//
//  gunwuha.c
//  GunwOS
//
//  Created by Artur Danielewski on 11.05.2023.
//

#ifdef _GUNWAPI_KERNEL

#include "../include/gunwuha.h"

struct gnwDeviceUHADesc uhaGetDesc(const size_t identifier,
                                   const enum gnwDeviceType type,
                                   const struct gnwDeviceUHA api) {
    return (struct gnwDeviceUHADesc) {
        identifier,
        type,
        api.system.desc,
        api.mem.desc,
        api.keyboard.desc,
        api.mouse.desc,
        api.fdc.desc,
        api.storCtrl.desc,
        api.charIn.desc,
        api.charOut.desc,
        api.display.desc,
        api.event.desc
    };
}

#endif // _GUNWAPI_KERNEL