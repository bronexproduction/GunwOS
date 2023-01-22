//
//  panic.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#include <stdgunw/string.h>
#include <stdgunw/utils.h>

// static const char * const title = "Oops!";

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
} CharColor;

struct k_vid_character {
    char c;
    char attr;
};

static const volatile ptr_t VIDEO_HW_MEM = (volatile ptr_t)0xb8000;
#define VIDEO_HW_ROWS   25
#define VIDEO_HW_COLS   80

#define VIDEO_HW_OFFSET(ROW, COL) ((volatile ptr_t)(VIDEO_HW_MEM + (COL + VIDEO_HW_COLS * ROW) * 2))
#define VIDEO_HW_END VIDEO_HW_OFFSET(VIDEO_HW_ROWS, VIDEO_HW_COLS)

static void switchToVGATextMode() {
    // Not implemented yet
}

static inline ptr_t charAt(size_t row, size_t col) {
    return VIDEO_HW_OFFSET(row, col);
}

static inline ptr_t attrAt(size_t row, size_t col) {
    return VIDEO_HW_OFFSET(row, col) + 1;
}

static int draw(const struct k_vid_character c, unsigned char x, unsigned char y) {
    if (x >= VIDEO_HW_COLS || y >= VIDEO_HW_ROWS) {
        return -1;
    }

    *(charAt(y,x)) = c.c;
    *(attrAt(y,x)) = c.attr;

    return 1;
}

static void clear(const struct k_vid_character bgChar) {
    for (int r = 0; r < VIDEO_HW_COLS; ++r) {
        for (int c = 0; c < VIDEO_HW_ROWS; ++c) {
            draw(bgChar, r, c);
        }
    }
}

static void strToChar(const char * const str, struct k_vid_character * const result, const char attributes, const size_t len) {
    if (!str || !result) {
        return;
    }
    
    for (size_t i = 0; i < len; ++i) {
        *(result + i) = (struct k_vid_character){ str[i], attributes };
    }
}

void k_oops(const char *reason) {
    switchToVGATextMode();

    struct k_vid_character bgChar;
    bgChar.c = 0;
    bgChar.attr = White | (Magenta << 4);

    clear(bgChar);

    size_t descriptionLength = strlen(reason);
    struct k_vid_character description[descriptionLength];
    strToChar(reason, description, bgChar.attr, descriptionLength);

    int padding = VIDEO_HW_COLS - descriptionLength;
    int row = MAX(0, padding) / 2;
    size_t column = 1;

    for (size_t i = 0; i < descriptionLength; ++i) {
        draw(description[i], row, column);

        ++row;
        if (row >= VIDEO_HW_COLS) {
            row = 0;
            ++column;
        }
    }

    // register uint_32 * stackPtr  __asm__ ("esp");
    for (size_t i = 0; i < 20; ++i) {
        // uint_32 val = *(stackPtr + sizeof(uint_32 *)*i);

        // c_trm_putin(val);
        // c_trm_putc(' ');
    } 

    while(1);
}