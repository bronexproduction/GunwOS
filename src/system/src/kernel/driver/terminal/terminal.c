//
//  terminal.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include <gunwdrv.h>
#include <stdgunw/mem.h>

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

static bool append(const char c) {
    if (c == '\n') {
        newline();
    } else if (c == '\b') {
        if (!(termData.cursorX || termData.cursorY)) {
            return 0;
        }

        back();
        append(' ');
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

static bool init() {
    return k_trm_vid_init();
}

static bool start() {
    clear();

    return true;
}

static bool isReadyToWrite() {
    return true;
}

static struct gnwDriverConfig desc() {
    return (struct gnwDriverConfig){ 
        /* init */ init,
        /* start */ start,
        /* isr */ nullptr,
        /* IRQ */ NULL
    };
}

static struct gnwDeviceUHA uha() {
    struct gnwDeviceUHA uha;

    uha.charOut.routine.isReadyToWrite = isReadyToWrite;
    uha.charOut.routine.write = append;

    return uha;
}

struct gnwDeviceDescriptor k_drv_terminal_descriptor() {
    return (struct gnwDeviceDescriptor) {
        /* type */ DEV_TYPE_CHAR_OUT,
        /* api */ uha(),
        /* driver */ (struct gnwDeviceDriver) {
            /* io */ (struct gnwDeviceIO) {
                /* busBase */ NULL
            },
            /* descriptor */ desc()
        },
        /* name */ "Default text mode terminal"
    };
}
