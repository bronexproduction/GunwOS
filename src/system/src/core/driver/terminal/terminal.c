//
//  terminal.c
//  GunwOS
//
//  Created by Artur Danielewski on 19.01.2018.
//

#include <stdgunw/types.h>
#include <stdgunw/string.h>
#include <stdgunw/math.h>
#include <stdgunw/mem.h>
#include "../display/video/video.h"

#define IO_GENERAL_FAILURE -1

const VideoCharColor    TERMINAL_CHAR_COLOR_DFLT    = White;
const VideoCharColor    TERMINAL_BG_COLOR_DFLT      = Black;
const unsigned char     TERMINAL_CMD_MAXLEN         = 255;

struct c_trm_terminal {
    char cursorX;
    char cursorY;
} c_trm_default;

void c_trm_clear() {
    c_trm_default.cursorX = 0;
    c_trm_default.cursorY = 0;

    k_vid_clear();
}

void c_trm_init() {
    c_trm_clear();
}

void c_trm_lineShift(struct c_trm_terminal * const trm) {
    uint_8 columns = k_vid_dimension(0);
    uint_8 rows = k_vid_dimension(1);
    point_t lastRowBegin = (point_t){ 0, rows - 1 };
    point_t lastRowEnd = (point_t){ columns - 1, rows - 1 };
    
    k_vid_shift(columns);
    k_vid_fill(lastRowBegin,
               lastRowEnd,
               (struct k_vid_character){ 0, 0 });

    trm->cursorY--;
}

static void c_trm_newline(struct c_trm_terminal * const trm) {
    trm->cursorX = 0;
    ++(trm->cursorY);

    while (trm->cursorY == k_vid_dimension(1)) {
        c_trm_lineShift(trm);
    }
}

static void c_trm_back(struct c_trm_terminal * const trm) {
    if (!(trm->cursorX)) {
        --(trm->cursorY);
        trm->cursorX = k_vid_dimension(0) - 1;
    } else {
        --(trm->cursorX);
    }
}

int c_trm_append(const char c) {
    if (c == '\n') {
        c_trm_newline(&c_trm_default);
    } else if (c == '\b') {
        if (!(c_trm_default.cursorX || c_trm_default.cursorY)) {
            return 0;
        }

        c_trm_back(&c_trm_default);
        c_trm_append(' ');
        c_trm_back(&c_trm_default);
    } else {
        char defaultAttr = k_vid_charAttr(TERMINAL_CHAR_COLOR_DFLT, TERMINAL_BG_COLOR_DFLT);

        const struct k_vid_character vChar = {c, defaultAttr};
        k_vid_draw(vChar, c_trm_default.cursorX, c_trm_default.cursorY);
    
        c_trm_default.cursorX++;
        
        if (c_trm_default.cursorX >= k_vid_dimension(0)) {
            c_trm_newline(&c_trm_default);
        }
    }

    return 1;
}

int c_trm_putc(const char c) {
     return c_trm_append(c);
}

int c_trm_puts(const char * const s) {
    int bWritten = 0;

    while (*(s + bWritten)) {
        if (c_trm_putc(*(s + bWritten))) {
            bWritten++;
        } else return IO_GENERAL_FAILURE;
    }

    return bWritten;
}

int c_trm_putsl(const char * const s, unsigned int l) {
    if (!l) {
        return 0;
    }

    int count = l;
    int bWritten = 0;

    while (*(s + bWritten) && l--) {
        if (c_trm_putc(*(s + bWritten))) {
            bWritten++;
        } else return IO_GENERAL_FAILURE;
    }

    if (bWritten != count) {
        return IO_GENERAL_FAILURE;
    }

    return bWritten;
}

// TODO: 64-bit version
int c_trm_putun_base(uint_32 i, uint_8 base) {
    char buffer[32];
    memnull(buffer, 32);
    size_t length = uint2str(i, buffer, base);

    if (!length) return IO_GENERAL_FAILURE;

    return c_trm_putsl(buffer, length);
}

// TODO: 64-bit version
int c_trm_putun(uint_32 i) {
    return c_trm_putun_base(i, 10);
}

// TODO: 64-bit version
int c_trm_putun_h(uint_32 i) {
    return c_trm_putun_base(i, 16);
}

// TODO: 64-bit version
int c_trm_putin(int_32 i) {
    char buffer[32];
    memnull(buffer, 32);
    size_t length = int2str(i, buffer);

    if (!length) return IO_GENERAL_FAILURE;

    return c_trm_putsl(buffer, length);
}
