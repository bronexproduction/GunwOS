//
//  video.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#ifndef VIDEO_H
#define VIDEO_H

#include <stdgunw/utils.h>
#include <stdgunw/types.h>

typedef enum c_vid_charColor {
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

static inline char c_vid_charAttr(const VideoCharColor main, const VideoCharColor bg) {
    return main | (bg << 4);
}

struct c_vid_character {
    char c;
    char attr;
};

unsigned char c_vid_dimension(int vertical);
int c_vid_draw(const struct c_vid_character c, unsigned char x, unsigned char y);
void c_vid_shift(const size_t charCount);
void c_vid_fill(const point_t start, const point_t end, const struct c_vid_character c);
void c_vid_clear();
void c_vid_strToChar(const char * const str, struct c_vid_character * const result, const char attributes, const size_t len);

#endif // VIDEO_H
