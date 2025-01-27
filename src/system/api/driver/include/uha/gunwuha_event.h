//
//  gunwuha_event.h
//  Universal Hardware Abstraction for event-emitting device
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2023.
//

#ifndef GUNWOS_GUNWUHA_EVENT_H
#define GUNWOS_GUNWUHA_EVENT_H

#include <uha/gunwuha_event_desc.h>
#include <gunwevent.h>

#ifndef _GUNWAPI_KERNEL

#define GNW_UHA_NO_EVENT GNW_UHA_EMPTY(EVENT)
#define GNW_UHA_EVENT_ROUTINE_EMPTY { 0 }

#endif // _GUNWAPI_KERNEL

/*
    Event emitting device routines
*/
struct gnwDeviceUHA_event_routine {
    void (*_unused)();
};

/*
    Event emitting device UHA
*/
struct gnwDeviceUHA_event {
    struct gnwDeviceUHA_event_desc desc;
    struct gnwDeviceUHA_event_routine routine;
};

#endif // GUNWOS_GUNWUHA_EVENT_H