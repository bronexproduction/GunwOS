//
//  mouse.c
//  GunwOS
//
//  Created by Artur Danielewski on 27.01.2025.  
//

#include "mouse.h"
#include "ops.h"
#include <gunwdrv.h>
#include <gunwoutput.h>
#include <gunwfug.h>
#include <gunwdevemitter.h>
#include <gunwinput.h>

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

static bool buttonPressed[2] = { 0 };

void emitMouseEvent(enum gnwMouseEventCode type, union gnwMouseEventData data) {
    enum gnwDeviceError err;
    struct gnwDeviceEvent event;

    event.type = type;
    event.data = (ptr_t)&data;
    event.dataSizeBytes = sizeof(union gnwMouseEventData);

    err = emit(MOUSE_DEVICE_ID, &event);
    if (err == GDE_NOT_FOUND) {
        logn("Mouse event ignored - no listener");
    } else if (err != GDE_NONE) {
        logn("Error emitting mouse event");
        fug(FUG_UNDEFINED);
    }
}

void emitMouseEvents(const struct eventDataPacket data) {
    if (data.state.buttonLeft != buttonPressed[GMK_LEFT]) {
        buttonPressed[GMK_LEFT] = data.state.buttonLeft;
        
        union gnwMouseEventData eventData;
        eventData.key = GMK_LEFT;

        emitMouseEvent(data.state.buttonLeft ? GMEC_KEY_DOWN : GMEC_KEY_UP, eventData);
    }
    if (data.state.buttonRight != buttonPressed[GMK_RIGHT]) {
        buttonPressed[GMK_RIGHT] = data.state.buttonRight;
        
        union gnwMouseEventData eventData;
        eventData.key = GMK_RIGHT;

        emitMouseEvent(data.state.buttonRight ? GMEC_KEY_DOWN : GMEC_KEY_UP, eventData);
    }
    if (data.movementX || data.movementY) {
        union gnwMouseEventData eventData;
        eventData.coordinates.x = data.movementX;
        eventData.coordinates.y = data.movementY;

        emitMouseEvent(GMEC_MOVEMENT, eventData);
    }
    
}
