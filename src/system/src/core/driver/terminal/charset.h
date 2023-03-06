//
//  charset.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include <stdgunw/types.h>

#define CHARSET_CODES_MAX 99

__attribute__((packed)) struct c_chs_mapping {
    char character;
};

const struct c_chs_mapping c_chs_default[CHARSET_CODES_MAX];

uint_8 c_chs_defines(const struct c_chs_mapping *map, const uint_8 c);
