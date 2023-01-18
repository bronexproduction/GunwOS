//
//  video.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#include <stdgunw/mem.h>
#include <gunwdisplay.h>
#include "../../log/log.h"
#include "../../kernel/error/panic.h"
#include "video.h"

struct gnwDeviceUHA_display_character frameBuffer[DISPLAY_ROWS * DISPLAY_COLS];

#define BUFFER_INDEX(ROW, COL) (COL + DISPLAY_COLS * ROW)
#define BUFFER_END (((ptr_t)frameBuffer) + BUFFER_INDEX(DISPLAY_ROWS, DISPLAY_COLS) * sizeof(struct gnwDeviceUHA_display_character)) 

static struct gnwTextDisplayHandle displayHandle;

bool c_vid_init() {
    struct gnwDisplayDescriptor desc;
    enum gnwDeviceError e = getTextDisplay(&desc);

    if (e) {
        OOPS("Error retrieving available text display");
        return true;
    }

    e = attachToTextDisplay(desc.identifier, &displayHandle);
    if (e) {
        LOG_FATAL("Unable to attach display");
        return true;
    }

    return false;
}

static void c_vid_push() {
    if (!displayHandle.update) {
        return;
    }

    enum gnwDisplayError e = displayHandle.update(frameBuffer);
    if (e) {
        LOG_FATAL("Error updating display buffer");
    }
}

int c_vid_draw(const struct gnwDeviceUHA_display_character c, unsigned char x, unsigned char y) {
    if (x >= DISPLAY_COLS || y >= DISPLAY_ROWS) {
        return -1;
    }

    frameBuffer[BUFFER_INDEX(y, x)] = c;
    
    c_vid_push();

    return 1;
}

void c_vid_shift(const size_t charCount) {
    ptr_t source = (ptr_t)frameBuffer + (charCount * sizeof(struct gnwDeviceUHA_display_character));
    size_t bytes = (source >= BUFFER_END) ? 0 : (BUFFER_END - source);
    if (!bytes) { return; }

    uint_8 buffer[bytes];
    memcopy(source, buffer, bytes);
    memcopy(buffer, frameBuffer, bytes);

    c_vid_push();
}

void c_vid_fill(const point_t start, const point_t end, const struct gnwDeviceUHA_display_character c) {
    if (start.x < 0 || start.x >= DISPLAY_COLS) { return; }
    if (start.y < 0 || start.y >= DISPLAY_ROWS) { return; }
    if (end.x < 0 || end.x >= DISPLAY_COLS) { return; }
    if (end.y < 0 || end.y >= DISPLAY_ROWS) { return; }

    const size_t lastIndex = BUFFER_INDEX(end.y, end.x);
    for (size_t index = BUFFER_INDEX(start.y, start.x); index <= lastIndex; ++index) {
        frameBuffer[index] = c;
    }

    c_vid_push();
}

void c_vid_clear() {
    const point_t start = { 0, 0 };
    const point_t end = { DISPLAY_COLS - 1, DISPLAY_ROWS - 1 };

    c_vid_fill(start, end, (struct gnwDeviceUHA_display_character){ 0, 0, 0 });
}
