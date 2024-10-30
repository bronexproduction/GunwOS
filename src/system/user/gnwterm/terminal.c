//
//  terminal.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include <mem.h>
#include <gunwipc.h>
#include <gunwrlp.h>
#include <gunwfug.h>

#include "video.h"

const VideoCharColor TERMINAL_CHAR_COLOR_DFLT = White;
const VideoCharColor TERMINAL_BG_COLOR_DFLT   = Black;

struct k_trm_terminal {
    char cursorX;
    char cursorY;
} termData;

static void lineShift() {
    uint_8 columns = k_vid_dimensions.x;
    uint_8 rows = k_vid_dimensions.y;
    point_t lastRowBegin = (point_t){ 0, rows - 1 };
    point_t lastRowEnd = (point_t){ columns - 1, rows - 1 };
    
    k_trm_vid_shift(columns);
    k_trm_vid_fill(lastRowBegin,
               lastRowEnd,
               (struct gnwDeviceUHA_display_character){ 0, 0, 0 });

    termData.cursorY--;
}

static void back() {
    if (!(termData.cursorX)) {
        --(termData.cursorY);
        termData.cursorX = k_vid_dimensions.x - 1;
    } else {
        --(termData.cursorX);
    }
}

static void newline() {
    termData.cursorX = 0;
    ++(termData.cursorY);

    while (termData.cursorY == k_vid_dimensions.y) {
        lineShift();
    }
}

static bool trm_append(const char c) {
    return true;

    if (c == '\n') {
        newline();
    } else if (c == '\b') {
        if (!(termData.cursorX || termData.cursorY)) {
            return false;
        }

        back();
        const bool result = trm_append(' ');
        if (!result) {
            return false;
        }
        back();
    } else {
        const struct gnwDeviceUHA_display_character vChar = {TERMINAL_BG_COLOR_DFLT, TERMINAL_CHAR_COLOR_DFLT, c};
        k_trm_vid_draw(vChar, termData.cursorX, termData.cursorY);
    
        termData.cursorX++;
        
        if (termData.cursorX >= k_vid_dimensions.x) {
            newline();
        }
    }

    return true;
}

static void clear() {
    memzero(&termData, sizeof(termData));
    k_trm_vid_clear();
}

static void ipcListener(const struct gnwIpcEndpointQuery * const query) {
    if (!query) {
        fug(FUG_NULLPTR);
        return;
    } 
    if (!query->data.ptr) {
        fug(FUG_INCONSISTENT);
        return;
    }
    if (query->data.bytes < 1) {
        fug(FUG_INCONSISTENT);
        return;
    }

    for (size_t i = 0; i < query->data.bytes; ++i) {
        const bool result = trm_append(query->data.ptr[i]);
        if (!result) {
            fug(FUG_UNDEFINED);
            return;
        }
    }
}

void dupa() {
    k_trm_vid_init();
    ipcRegister("t0", ipcListener, false, 0);
    clear();

    runLoopStart();
}
