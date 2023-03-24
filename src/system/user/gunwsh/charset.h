//
//  charset.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include <types.h>

#define CHARSET_CODES_MAX 99

__attribute__((packed)) struct user_cli_chs_mapping {
    char character;
};

const struct user_cli_chs_mapping user_cli_chs_default[CHARSET_CODES_MAX];

uint_8 user_cli_chs_defines(const struct user_cli_chs_mapping *map, const uint_8 c);
