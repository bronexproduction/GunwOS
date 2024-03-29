//
//  kbdmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef GUNWOS_API_KBDMGR_H
#define GUNWOS_API_KBDMGR_H

#include <defs.h>
#include <gunwdevtypes.h>
#include <gunwkeyboard.h>

#define KBDMGR_PATH_ATTACH "km/a"
#define KBDMGR_PATH_EVENT ":/km/e"

struct gnwKeyboardManagerAttachToKeyboardResult {
    enum gnwDeviceError error;
};

struct gnwKeyboardManagerEventQuery {
    struct gnwKeyboardEvent keyboardEvent;
};

#endif // GUNWOS_API_KBDMGR_H
