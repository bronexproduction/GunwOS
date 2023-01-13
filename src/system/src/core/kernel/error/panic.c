//
//  panic.c
//  GunwOS
//
//  Created by Artur Danielewski on 04.02.2021.
//

#include "../../driver/display/video/video.h"
#include <stdgunw/string.h>

// static const char * const title = "Oops!";

void k_oops(const char *reason) {
    
    k_vid_clear();

    struct k_vid_character bgChar;
    bgChar.c = 0;
    bgChar.attr = k_vid_charAttr(White, Magenta);

    for (int r = 0; r < k_vid_dimension(0); ++r) {
        for (int c = 0; c < k_vid_dimension(1); ++c) {
            k_vid_draw(bgChar, r, c);
        }
    }

    size_t descriptionLength = strlen(reason);
    struct k_vid_character description[descriptionLength];
    k_vid_strToChar(reason, description, bgChar.attr, descriptionLength);

    int padding = k_vid_dimension(0) - descriptionLength;
    size_t row = MAX(0, padding) / 2;
    size_t column = 1;

    for (size_t i = 0; i < descriptionLength; ++i) {
        k_vid_draw(description[i], row, column);

        ++row;
        if (row >= k_vid_dimension(0)) {
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