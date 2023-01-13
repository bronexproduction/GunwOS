//
//  charset.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include "../../../../lib/stdgunw/types.h"

#define CHARSET_CODES_MAX 99

__attribute__((packed)) struct k_chs_mapping {
    char character;
};

const struct k_chs_mapping k_chs_default[CHARSET_CODES_MAX];

uint_8 k_chs_defines(const struct k_chs_mapping *map, const uint_8 c);
