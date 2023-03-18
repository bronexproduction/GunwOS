//
//  gunwuha_event.h
//  Universal Hardware Abstraction for events emitting device
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2023.
//

#ifndef GUNWOS_GUNWUHA_EVENT_H
#define GUNWOS_GUNWUHA_EVENT_H

#include <uha/gunwuha_event_desc.h>
#include <gunwevent.h>

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