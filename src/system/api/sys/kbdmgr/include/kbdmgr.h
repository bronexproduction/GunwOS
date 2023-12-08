//
//  kbdmgr.h
//  GunwOS
//
//  Created by Artur Danielewski on 08.12.2023.
//

#ifndef GUNWOS_API_KBDMGR_H
#define GUNWOS_API_KBDMGR_H

#include <gunwdisplay.h>
#include <defs.h>

#define DISPMGR_PATH_GET "dm/g"
#define DISPMGR_PATH_ATTACH "dm/a"
#define DISPMGR_PATH_PUSH "dm/p"

#define DISPMGR_PUSH_MAX_BYTES KiB(6)

struct gnwDisplayManagerGetDisplayQuery {
    enum gnwDisplayType type;
};

struct gnwDisplayManagerGetDisplayResult {
    enum gnwDeviceError error;
    struct gnwDisplayDescriptor displayDescriptor;
};

struct gnwDisplayManagerAttachToDisplayQuery {
    enum gnwDisplayType type;
    struct gnwDisplayDescriptor displayDescriptor;
};

struct gnwDisplayManagerAttachToDisplayResult {
    enum gnwDeviceError error;
};

struct gnwDisplayManagerPushFrameQuery {
    size_t displayId;
    uint_8 frameBuffer[DISPMGR_PUSH_MAX_BYTES];
    range_addr_t inputRange;
};

struct gnwDisplayManagerPushFrameResult {
    enum gnwDeviceError error;
};

#endif // GUNWOS_API_KBDMGR_H
