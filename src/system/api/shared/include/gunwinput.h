//
//  gunwinput.h
//  GunwOS
//
//  Created by Artur Danielewski on 07.03.2023.
//

#ifndef GUNWOS_GUNWINPUT_H
#define GUNWOS_GUNWINPUT_H

#include <types.h>

/*
    Event type code for keyboard event listener
*/
enum gnwKeyboardEventCode {
    GKEC_KEY_DOWN,
    GKEC_KEY_UP
};
_Static_assert(sizeof(enum gnwKeyboardEventCode) == sizeof(int_32), "Unexpected enum gnwKeyboardEventCode size");

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
    Macros for keyboard and mouse event listener signature
*/
#define GNW_KEYBOARD_EVENT_LISTENER(NAME) void NAME (const struct gnwKeyboardEvent * const event)
#define GNW_MOUSE_EVENT_LISTENER(NAME) void NAME (const struct gnwMouseEvent * const event)

/*
    Keyboard listener callback data definition
*/
struct gnwKeyboardEvent {
    enum gnwKeyboardEventCode code;
    char key;
};

/*
    Mouse event data
*/
union gnwMouseEventData {
    enum gnwMouseKey key;
    point_t coordinates;
};

/*
    Mouse listener callback data definition
*/
struct gnwMouseEvent {
    enum gnwMouseEventCode code;
    union gnwMouseEventData data;
};

/*
    Keyboard and mouse listeners callback type definition
*/
typedef __attribute__((cdecl)) void (*gnwKeyboardEventListener)(const struct gnwKeyboardEvent * const);
typedef __attribute__((cdecl)) void (*gnwMouseEventListener)(const struct gnwMouseEvent * const);

/*
    Attaching to the keyboard and mouse

    Params:
        * keyboardListener - keyboard event handler
        * mouseListener - mouse event handler

    Result:
        * Error value indicating what went wrong while attaching

    Note: Only the process attached to the input can receive key and mouse status updates
*/
enum gnwDeviceError attachToInput(const gnwKeyboardEventListener keyboardListener,
                                  const gnwMouseEventListener mouseListener);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWINPUT_H
