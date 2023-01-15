//
//  video.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#include <stdgunw/mem.h>
#include "video.h"

#define DISPLAY_ROWS            25
#define DISPLAY_COLS            80
#define DISPLAY_BYTES_PER_CHAR  2

static uint_8 frameBuffer[DISPLAY_ROWS * DISPLAY_COLS * DISPLAY_BYTES_PER_CHAR];

#define BUFFER_OFFSET(ROW, COL) ((volatile ptr_t)(frameBuffer + (COL + DISPLAY_COLS * ROW) * 2))
#define BUFFER_END BUFFER_OFFSET(DISPLAY_ROWS, DISPLAY_COLS)

unsigned char c_vid_dimension(int vertical) {
    return vertical ? DISPLAY_ROWS : DISPLAY_COLS;
}

static inline ptr_t charAt(size_t row, size_t col) {
    return BUFFER_OFFSET(row, col);
}

static inline ptr_t attrAt(size_t row, size_t col) {
    return BUFFER_OFFSET(row, col) + 1;
}

int c_vid_draw(const struct c_vid_character c, unsigned char x, unsigned char y) {
    if (x >= DISPLAY_COLS || y >= DISPLAY_ROWS) {
        return -1;
    }

    *(charAt(y,x)) = c.c;
    *(attrAt(y,x)) = c.attr;

    return 1;
}

void c_vid_shift(const size_t charCount) {
    ptr_t source = frameBuffer + (charCount * 2);
    size_t bytes = (source >= BUFFER_END) ? 0 : (BUFFER_END - source);
    if (!bytes) { return; }

    uint_8 buffer[bytes];
    memcopy(source, buffer, bytes);
    memcopy(buffer, frameBuffer, bytes);
}

void c_vid_fill(const point_t start, const point_t end, const struct c_vid_character c) {
    if (start.x < 0 || start.x >= DISPLAY_COLS) { return; }
    if (start.y < 0 || start.y >= DISPLAY_ROWS) { return; }
    if (end.x < 0 || end.x >= DISPLAY_COLS) { return; }
    if (end.y < 0 || end.y >= DISPLAY_ROWS) { return; }

    const volatile ptr_t lastOffset = charAt(end.y, end.x);
    for (volatile ptr_t offset = charAt(start.y, start.x); offset <= lastOffset; offset += 2) {
        *offset = c.c;
        *(offset + 1) = c.attr;
    }
}

void c_vid_clear() {
    const point_t start = { 0, 0 };
    const point_t end = { c_vid_dimension(0) - 1, c_vid_dimension(1) - 1 };

    c_vid_fill(start, end, (struct c_vid_character){ 0, 0 });
}

void c_vid_strToChar(const char * const str, struct c_vid_character * const result, const char attributes, const size_t len) {
    if (!str || !result) {
        return;
    }
    
    for (size_t i = 0; i < len; ++i) {
        *(result + i) = (struct c_vid_character){ str[i], attributes };
    }
}
