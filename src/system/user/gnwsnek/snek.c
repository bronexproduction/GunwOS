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
#include <gunwdisplay.h>

#warning TAKE IT FROM DISPLAY INSTEAD (requires dynamic memory allocation)
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 80

point_t vid_dimensions = { DISPLAY_COLS, DISPLAY_ROWS };
struct gnwDeviceUHA_display_character frameBuffer[DISPLAY_COLS * DISPLAY_ROWS];
static struct gnwTextDisplayHandle displayHandle;

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

    for (size_t i = 0; i < DISPLAY_COLS * DISPLAY_ROWS; ++i) {
        frameBuffer[i].bgColor = 1;
        frameBuffer[i].charColor = 8;
        frameBuffer[i].character = 49;
    }
    pushFrame();

    size_t i = 0;

    for(; i < 1000000000; ++i);

    for (size_t i = 0; i < DISPLAY_COLS * DISPLAY_ROWS; ++i) {
        frameBuffer[i].bgColor = 2;
        frameBuffer[i].charColor = 4;
        frameBuffer[i].character = 50;
    }
    pushFrame();
}
