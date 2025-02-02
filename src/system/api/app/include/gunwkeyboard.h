//
//  gunwkeyboard.h
//  GunwOS
//
//  Created by Artur Danielewski on 07.03.2023.
//

#ifndef GUNWOS_GUNWKEYBOARD_H
#define GUNWOS_GUNWKEYBOARD_H

#include <types.h>

/*
    Event type code for keyboard event listener
*/
enum gnwKeyboardEventCode {
    GKEC_KEY_DOWN,
    GKEC_KEY_UP
};
_Static_assert(sizeof(enum gnwKeyboardEventCode) == sizeof(int_32), "Unexpected enum gnwKeyboardEventCode size");

#ifndef _GUNWAPI_KERNEL

/*
    Macro for keyboard event listener signature
*/
#define GNW_KEYBOARD_EVENT_LISTENER(NAME) void NAME (const struct gnwKeyboardEvent * const event)

/*
    Listener callback data definition
*/
struct gnwKeyboardEvent {
    enum gnwKeyboardEventCode code;
    char key;
};

/*
    Listener callback type definition
*/
typedef __attribute__((cdecl)) void (*gnwKeyboardEventListener)(const struct gnwKeyboardEvent * const);

/*
    Attaching to the keyboard

    Params:
        * listener - keyboard event handler

    Result:
        * Error value indicating what went wrong while attaching

    Note: Only the process attached to the keyboard can receive key status updates
*/
enum gnwDeviceError attachToKeyboard(const gnwKeyboardEventListener listener);

#endif // _GUNWAPI_KERNEL

#endif // GUNWOS_GUNWKEYBOARD_H
