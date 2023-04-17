//
//  terminal.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include <driver/gunwdrv.h>
#include <mem.h>
#include <gunwipc.h>

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
    if (c == '\n') {
        newline();
    } else if (c == '\b') {
        if (!(termData.cursorX || termData.cursorY)) {
            return 0;
        }

        back();
        trm_append(' ');
        back();
    } else {
        const struct gnwDeviceUHA_display_character vChar = {TERMINAL_BG_COLOR_DFLT, TERMINAL_CHAR_COLOR_DFLT, c};
        k_trm_vid_draw(vChar, termData.cursorX, termData.cursorY);
    
        termData.cursorX++;
        
        if (termData.cursorX >= k_vid_dimensions.x) {
            newline();
        }
    }

    return 1;
}

static void clear() {
    memnull(&termData, sizeof(termData));
    k_trm_vid_clear();
}

static void ipcListener(const procId_t procId, const char c) {
    trm_append(c);
}

void dupa() {
    k_trm_vid_init();
    ipcRegister("t0", GIAS_ALL, (gnwEventListener_32_8)ipcListener);
    clear();

    runLoopStart();
}
