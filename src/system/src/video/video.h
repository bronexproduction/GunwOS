//
//  video.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#ifndef SYSTEM_VIDEO_H
#define SYSTEM_VIDEO_H

#include "../../../lib/stdgunw/utils.h"
#include "../../../lib/stdgunw/types.h"

typedef enum k_vid_charColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    Gray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
} VideoCharColor;

static inline char k_vid_charAttr(const VideoCharColor main, const VideoCharColor bg) {
    return main | (bg << 4);
}

struct k_vid_character {
    char c;
    char attr;
};

unsigned char k_vid_dimension(int vertical);
int k_vid_draw(const struct k_vid_character c, unsigned char x, unsigned char y);
void k_vid_shift(const size_t charCount);
void k_vid_fill(const point_t start, const point_t end, const struct k_vid_character c);
void k_vid_clear();
void k_vid_strToChar(const char * const str, struct k_vid_character * const result, const char attributes, const size_t len);

#endif // SYSTEM_VIDEO_H
