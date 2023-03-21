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
    Macro for keyboard event listener signature
*/
#define GNW_KEYBOARD_EVENT_LISTENER(NAME) void NAME (enum gnwKeyboardEventCode code, int_8 data)

/*
    Event type code for keyboard event listener
*/
enum gnwKeyboardEventCode {
    GKEC_KEY_DOWN,
    GKEC_KEY_UP
};
_Static_assert(sizeof(enum gnwKeyboardEventCode) == sizeof(int_32), "Unexpected enum gnwKeyboardEventCode size");

/*
    Listener callback type definition
*/
typedef gnwEventListener_32_8 gnwKeyboardEventListener;

#endif // GUNWOS_GUNWKEYBOARD_H
