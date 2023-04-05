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
#include <gunwevent.h>

/*
    Event emitting device descriptor
*/
struct gnwDeviceUHA_event_desc {
    /*
        Event data format

        Determines the type of listener data parameter
    */
    enum gnwEventFormat eventDataFormat;
};

#endif // GUNWOS_GUNWUHA_EVENT_DESC_H
