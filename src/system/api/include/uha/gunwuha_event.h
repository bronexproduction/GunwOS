//
//  gunwuha_event.h
//  Universal Hardware Abstraction for events emitting device
//  GunwOS
//
//  Created by Artur Danielewski on 08.03.2023.
//

#ifndef GUNWOS_GUNWUHA_EVENT_H
#define GUNWOS_GUNWUHA_EVENT_H

#include <stdgunw/types.h>
#include <gunwdevevent.h>

/*
    Event emitting device descriptor
*/
struct gnwDeviceUHA_event_desc {
    /*
        Event data format

        Determines the type of listener data parameter
    */
    enum gnwDeviceEventFormat eventDataFormat;
};

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