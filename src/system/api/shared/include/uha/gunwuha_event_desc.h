//
//  gunwuha_event_desc.h
//  Universal Hardware Abstraction for event-emitting device
//  GunwOS
//
//  Created by Artur Danielewski on 09.03.2023.
//

#ifndef GUNWOS_GUNWUHA_EVENT_DESC_H
#define GUNWOS_GUNWUHA_EVENT_DESC_H

#include <types.h>
#include "../gunwevent.h"

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_EVENT_DESC_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

/*
    Event emitting device descriptor
*/
struct gnwDeviceUHA_event_desc {
    uint_32 _unused;
};

#endif // GUNWOS_GUNWUHA_EVENT_DESC_H
