//
//  video.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#include "video.h"
#include "../../../lib/stdgunw/mem.h"

const volatile ptr_t VIDEO_HW_MEM   = (volatile ptr_t)0xb8000;
const int VIDEO_HW_ROWS             = 25;
const int VIDEO_HW_COLS             = 80;

#define VIDEO_HW_OFFSET(ROW, COL) ((volatile ptr_t)(VIDEO_HW_MEM + (COL + VIDEO_HW_COLS * ROW) * 2))
#define VIDEO_HW_END VIDEO_HW_OFFSET(VIDEO_HW_ROWS, VIDEO_HW_COLS)

unsigned char k_vid_dimension(int vertical) {
    return vertical ? VIDEO_HW_ROWS : VIDEO_HW_COLS;
}

static inline ptr_t charAt(size_t row, size_t col) {
    return VIDEO_HW_OFFSET(row, col);
}

static inline ptr_t attrAt(size_t row, size_t col) {
    return VIDEO_HW_OFFSET(row, col) + 1;
}

int k_vid_draw(const struct k_vid_character c, unsigned char x, unsigned char y) {
    if (x >= VIDEO_HW_COLS || y >= VIDEO_HW_ROWS) {
        return -1;
    }

    *(charAt(y,x)) = c.c;
    *(attrAt(y,x)) = c.attr;

    return 1;
}

void k_vid_shift(const size_t charCount) {
    ptr_t source = VIDEO_HW_MEM + (charCount * 2);
    size_t bytes = (source >= VIDEO_HW_END) ? 0 : (VIDEO_HW_END - source);
    if (!bytes) { return; }

    uint_8 buffer[bytes];
    memcopy(source, buffer, bytes);
    memcopy(buffer, VIDEO_HW_MEM, bytes);
}

void k_vid_fill(const point_t start, const point_t end, const struct k_vid_character c) {
    if (start.x < 0 || start.x >= VIDEO_HW_COLS) { return; }
    if (start.y < 0 || start.y >= VIDEO_HW_ROWS) { return; }
    if (end.x < 0 || end.x >= VIDEO_HW_COLS) { return; }
    if (end.y < 0 || end.y >= VIDEO_HW_ROWS) { return; }

    const volatile ptr_t lastOffset = charAt(end.y, end.x);
    for (volatile ptr_t offset = charAt(start.y, start.x); offset <= lastOffset; offset += 2) {
        *offset = c.c;
        *(offset + 1) = c.attr;
    }
}

void k_vid_clear() {
    const point_t start = { 0, 0 };
    const point_t end = { k_vid_dimension(0) - 1, k_vid_dimension(1) - 1 };

    k_vid_fill(start, end, (struct k_vid_character){ 0, 0 });
}

void k_vid_strToChar(const char * const str, struct k_vid_character * const result, const char attributes, const size_t len) {
    if (!str || !result) {
        return;
    }
    
    for (size_t i = 0; i < len; ++i) {
        *(result + i) = (struct k_vid_character){ str[i], attributes };
    }
}
