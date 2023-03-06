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

#define DISPLAY_ROWS 25
#define DISPLAY_COLS 80

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

int k_vid_draw(const struct gnwDeviceUHA_display_character c, unsigned char x, unsigned char y);
void k_vid_shift(const size_t charCount);
void k_vid_fill(const point_t start, const point_t end, const struct gnwDeviceUHA_display_character c);
void k_vid_clear();

#endif // VIDEO_H
