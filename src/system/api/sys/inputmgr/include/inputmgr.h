//
//  inputmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef GUNWOS_API_INPUTMGR_H
#define GUNWOS_API_INPUTMGR_H

#include <defs.h>
#include <gunwdevtypes.h>
#include <gunwinput.h>

#define INPUTMGR_PATH_ATTACH            "in/a"
#define INPUTMGR_PATH_KEYBOARD_EVENT    ":/in/k"
#define INPUTMGR_PATH_MOUSE_EVENT       ":/in/m"

struct gnwInputManagerAttachResult {
    enum gnwDeviceError error;
};

struct gnwInputManagerKeyboardEventQuery {
    struct gnwKeyboardEvent keyboardEvent;
};

struct gnwInputManagerMouseEventQuery {
    struct gnwMouseEvent mouseEvent;
};

#endif // GUNWOS_API_INPUTMGR_H
