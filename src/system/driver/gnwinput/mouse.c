//
//  mouse.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2025.  
//

#include "mouse.h"
#include "ops.h"
#include <gunwdrv.h>
#include <gunwlog.h>
#include <gunwfug.h>
#include <gunwdevemitter.h>
#include <gunwmouse.h>

#define TRY(OP, ON_FAIL) if (!OP()) { ON_FAIL; return; }
#define INIT_TRY(OP) TRY(OP, drvInitReport(MOUSE_DEVICE_ID, false))
#define START_TRY(OP) TRY(OP, drvStartReport(MOUSE_DEVICE_ID, false))

extern const size_t MOUSE_DEVICE_ID;

void init_mouse() {
    INIT_TRY(inputEnable);
    INIT_TRY(reset);
    INIT_TRY(setDefaults);

    drvInitReport(MOUSE_DEVICE_ID, true);
}

void start_mouse() {
    START_TRY(enablePacketStreaming);
    START_TRY(enableInterrupt);

    drvStartReport(MOUSE_DEVICE_ID, true);
}

void emitMouseEvent(const struct movementDataPacket data) {
    enum gnwDeviceError err;
    struct gnwDeviceEvent event;
    event.type = type;
    event.data = (ptr_t)&data;
    // event.dataSizeBytes = sizeof(char);

    // err = emit(KEYBOARD_DEVICE_ID, &event);
    // if (err == GDE_NOT_FOUND) {
    //     log("Keyboard event ignored - no listener");
    // } else if (err != GDE_NONE) {
    //     log("Error emitting keyboard event");
    //     fug(FUG_UNDEFINED);
    // }
}
