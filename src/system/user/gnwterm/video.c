//
//  video.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#include <defs.h>
#include <mem.h>
#include <gunwctrl.h>
#include <gunwfug.h>
#include <gunwmem.h>
#include "video.h"

struct gnwDeviceUHA_display_character *frameBuffer = nullptr;

#define BUFFER_INDEX(ROW, COL) (COL + displayHandle.descriptor.dimensions.x * ROW)
#define BUFFER_END ( \
    ((ptr_t)frameBuffer) + \
    BUFFER_INDEX( \
        displayHandle.descriptor.dimensions.y, \
        displayHandle.descriptor.dimensions.x \
    ) * sizeof(struct gnwDeviceUHA_display_character) \
) 

struct gnwTextDisplayHandle displayHandle;

bool k_trm_vid_init() {
    struct gnwDisplayDescriptor desc;
    enum gnwDeviceError e = getTextDisplay(&desc);

    if (!desc.dimensions.x ||
        !desc.dimensions.y) {
        fug(FUG_UNDEFINED);
        return false;
    }
    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    e = attachToTextDisplay(&desc, &displayHandle);
    if (e != GDE_NONE) {
        fug(FUG_UNDEFINED);
        return false;
    }

    frameBuffer = (struct gnwDeviceUHA_display_character *)memPlz(
        sizeof(struct gnwDeviceUHA_display_character) * 
        desc.dimensions.x * 
        desc.dimensions.y
    );
    if (!frameBuffer) {
        fug(FUG_UNDEFINED);
        displayHandle.invalidate(&displayHandle);
        return false;
    }

    return true;
}

static void k_vid_push() {
    if (!displayHandle.update) {
        return;
    }

    enum gnwDeviceError e;
    while ((e = displayHandle.update(&displayHandle, frameBuffer)) == GDE_BUSY) {
        yield();
    }
    if (e) {
        fug(FUG_UNDEFINED);
        displayHandle.invalidate(&displayHandle);
        memThx(frameBuffer);
    }
}

int k_trm_vid_draw(const struct gnwDeviceUHA_display_character c, unsigned char x, unsigned char y) {
    if (x >= displayHandle.descriptor.dimensions.x ||
        y >= displayHandle.descriptor.dimensions.y) {
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
    if (start.x < 0 || start.x >= displayHandle.descriptor.dimensions.x) { return; }
    if (start.y < 0 || start.y >= displayHandle.descriptor.dimensions.y) { return; }
    if (end.x < 0 || end.x >= displayHandle.descriptor.dimensions.x) { return; }
    if (end.y < 0 || end.y >= displayHandle.descriptor.dimensions.y) { return; }

    const size_t lastIndex = BUFFER_INDEX(end.y, end.x);
    for (size_t index = BUFFER_INDEX(start.y, start.x); index <= lastIndex; ++index) {
        frameBuffer[index] = c;
    }

    k_vid_push();
}

void k_trm_vid_clear() {
    const point_t start = { 0, 0 };
    const point_t end = { displayHandle.descriptor.dimensions.x - 1,
                          displayHandle.descriptor.dimensions.y - 1 };

    k_trm_vid_fill(start, end, (struct gnwDeviceUHA_display_character){ 0, 0, 0 });
}
