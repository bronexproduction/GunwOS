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
#include <gunwkeyboard.h>
#include <gunwmouse.h>

#define INPUTMGR_PATH_ATTACH    "im/a"
#define INPUTMGR_PATH_EVENT     ":/im/e"

struct gnwInputManagerAttachToKeyboardResult {
    enum gnwDeviceError error;
};

struct gnwInputManagerKeyboardEventQuery {
    struct gnwKeyboardEvent keyboardEvent;
};

struct gnwInputManagerMouseEventQuery {
    struct gnwMouseEvent mouseEvent;
};

#endif // GUNWOS_API_INPUTMGR_H
