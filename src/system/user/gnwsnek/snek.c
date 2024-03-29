//
//  snek.c
//  GunwOS
//
//  Created by Artur Danielewski on 22.06.2023.
//

#include <types.h>
#include <defs.h>
#include <gunwdev.h>
#include <gunwfug.h>
#include <gunwipc.h>
#include <gunwrlp.h>
#include <gunwdisplay.h>
#include <gunwkeyboard.h>

#warning TAKE IT FROM DISPLAY INSTEAD (requires dynamic memory allocation)
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 80

point_t vid_dimensions = { DISPLAY_COLS, DISPLAY_ROWS };
struct gnwDeviceUHA_display_character frameBuffer[DISPLAY_COLS * DISPLAY_ROWS];
static struct gnwTextDisplayHandle displayHandle;

static size_t global_i = 1;

static GNW_KEYBOARD_EVENT_LISTENER(onKeyboardEvent) {
    if (event->code != GKEC_KEY_DOWN) {
        ++global_i;
    }
}

static void onSessionDestroy(const struct gnwIpcEndpointQuery * const query) {
    (void)query;
    fug(FUG_INCONSISTENT);
}

static void init() {
    struct gnwDisplayDescriptor desc;
    enum gnwDeviceError e = getTextDisplay(&desc);

    if (desc.dimensions.x != vid_dimensions.x ||
        desc.dimensions.y != vid_dimensions.y) {
        fug(FUG_UNDEFINED);
        // OOPS("Unsupported display dimensions");
    }
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Error retrieving available text display");
    }

    e = attachToTextDisplay(&desc, &displayHandle);
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Unable to attach display");
    }

    ipcSessionDestroyListener = onSessionDestroy;
    e = attachToKeyboard(onKeyboardEvent);
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Unable to attach display");
    }
}

static void pushFrame() {
    if (!displayHandle.update) {
        return;
    }

    enum gnwDeviceError e = displayHandle.update(&displayHandle, frameBuffer);
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Error updating display buffer");
        displayHandle.update = nullptr;
    }
}

void dupa() {
    init();

    while (global_i < 10) {
        for (size_t i = 0; i < DISPLAY_COLS * DISPLAY_ROWS; ++i) {
            frameBuffer[i].bgColor = global_i;
            frameBuffer[i].charColor = global_i * 2;
            frameBuffer[i].character = 48 + global_i;
        }
        pushFrame();
        runLoopHandle();
    }
}
