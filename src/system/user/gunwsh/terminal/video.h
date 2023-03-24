//
//  video.h
//  GunwOS
//
//  Created by Artur Danielewski on 18.01.2018.
//

#ifndef VIDEO_H
#define VIDEO_H

#include <utils.h>
#include <types.h>

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

extern point_t k_vid_dimensions;

bool k_trm_vid_init();

int k_trm_vid_draw(const struct gnwDeviceUHA_display_character c, unsigned char x, unsigned char y);
void k_trm_vid_shift(const size_t charCount);
void k_trm_vid_fill(const point_t start, const point_t end, const struct gnwDeviceUHA_display_character c);
void k_trm_vid_clear();

#endif // VIDEO_H
