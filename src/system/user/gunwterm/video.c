//
//  video.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#include <mem.h>
// #include <error/panic.h>
#include <gunwfug.h>
#include <defs.h>
#include "video.h"

#warning TAKE IT FROM DISPLAY INSTEAD (requires dynamic memory allocation)
#define DISPLAY_ROWS 25
#define DISPLAY_COLS 80

point_t k_vid_dimensions = { DISPLAY_COLS, DISPLAY_ROWS };
struct gnwDeviceUHA_display_character frameBuffer[DISPLAY_COLS * DISPLAY_ROWS];

#define BUFFER_INDEX(ROW, COL) (COL + k_vid_dimensions.x * ROW)
#define BUFFER_END (((ptr_t)frameBuffer) + BUFFER_INDEX(k_vid_dimensions.y, k_vid_dimensions.x) * sizeof(struct gnwDeviceUHA_display_character)) 

static struct gnwTextDisplayHandle displayHandle;

bool k_trm_vid_init() {
    struct gnwDisplayDescriptor desc;
    enum gnwDeviceError e = getTextDisplay(&desc);

    if (desc.dimensions.x != k_vid_dimensions.x ||
        desc.dimensions.y != k_vid_dimensions.y) {
        fug(FUG_UNDEFINED);
        // OOPS("Unsupported display dimensions");
        return false;
    }
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Error retrieving available text display");
        return false;
    }

    e = attachToTextDisplay(&desc, &displayHandle);
    if (e) {
        fug(FUG_UNDEFINED);
        // OOPS("Unable to attach display");
        return false;
    }

    return true;
}

static void k_vid_push() {
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

int k_trm_vid_draw(const struct gnwDeviceUHA_display_character c, unsigned char x, unsigned char y) {
    if (x >= k_vid_dimensions.x || y >= k_vid_dimensions.y) {
        return -1;
    }

    frameBuffer[BUFFER_INDEX(y, x)] = c;
    
    k_vid_push();

    return 1;
}

void k_trm_vid_shift(const size_t charCount) {
    ptr_t source = (ptr_t)frameBuffer + (charCount * sizeof(struct gnwDeviceUHA_display_character));
    size_t bytes = (source >= BUFFER_END) ? 0 : (BUFFER_END - source);
    if (!bytes) { return; }

    uint_8 buffer[bytes];
    memcopy(source, buffer, bytes);
    memcopy(buffer, frameBuffer, bytes);

    k_vid_push();
}

void k_trm_vid_fill(const point_t start, const point_t end, const struct gnwDeviceUHA_display_character c) {
    if (start.x < 0 || start.x >= k_vid_dimensions.x) { return; }
    if (start.y < 0 || start.y >= k_vid_dimensions.y) { return; }
    if (end.x < 0 || end.x >= k_vid_dimensions.x) { return; }
    if (end.y < 0 || end.y >= k_vid_dimensions.y) { return; }

    const size_t lastIndex = BUFFER_INDEX(end.y, end.x);
    for (size_t index = BUFFER_INDEX(start.y, start.x); index <= lastIndex; ++index) {
        frameBuffer[index] = c;
    }

    k_vid_push();
}

void k_trm_vid_clear() {
    const point_t start = { 0, 0 };
    const point_t end = { k_vid_dimensions.x - 1, k_vid_dimensions.y - 1 };

    k_trm_vid_fill(start, end, (struct gnwDeviceUHA_display_character){ 0, 0, 0 });
}
