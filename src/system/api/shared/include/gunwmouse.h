//
//  gunwmouse.h
//  GunwOS
//
//  Created by Artur Danielewski on 30.07.2025.
//

#ifndef GUNWOS_GUNWMOUSE_H
#define GUNWOS_GUNWMOUSE_H

#include <types.h>

/*
    Event type code for mouse event listener
*/
enum gnwMouseEventCode {
    GMEC_KEY_DOWN,
    GMEC_KEY_UP,
    GMEC_MOVEMENT
};
_Static_assert(sizeof(enum gnwMouseEventCode) == sizeof(int_32), "Unexpected enum gnwMouseEventCode size");

/*
    Mouse key code
*/
enum gnwMouseKey {
    GMK_LEFT,
    GMK_RIGHT
};

#ifndef _GUNWAPI_KERNEL

/*
    Macro for mouse event listener signature
*/
#define GNW_MOUSE_EVENT_LISTENER(NAME) void NAME (const struct gnwMouseEvent * const event)

/*
    Mouse event data
*/
union gnwMouseEventData {
    enum gnwMouseKey key;
    point_t coordinates;
};

/*
    Listener callback data definition
*/
struct gnwMouseEvent {
    enum gnwMouseEventCode code;
    union gnwMouseEventData data;
};

/*
    Listener callback type definition
*/
typedef __attribute__((cdecl)) void (*gnwMouseEventListener)(const struct gnwMouseEvent * const);

/*
    Attaching to the mouse

    Params:
        * listener - mouse event handler

    Result:
        * Error value indicating what went wrong while attaching

    Note: Mouse updates are exclusive to the attached process
*/
enum gnwDeviceError attachToMouse(const gnwMouseEventListener listener);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWMOUSE_H
