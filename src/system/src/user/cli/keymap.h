//
//  keymap.h
//  GunwOS
//
//  Created by Artur Danielewski on 20.03.2020.
//

#include <types.h>

#define SCANCODES_MAX 128

enum user_cli_kmp_modifier {
    SHIFT   = 1,
    CTRL    = 1 << 1,
    ALT     = 1 << 2
};

enum user_cli_kmp_special {
    ESC     = 1,
    F1      = 2,
    F2      = 3,
    F3      = 4,
    F4      = 5,
    F5      = 6,
    F6      = 7,
    F7      = 8,
    F8      = 9,
    F9      = 10,
    F10     = 11,
    F11     = 12,
    F12     = 13,
    CAPS    = 14
};

__attribute__((packed)) struct user_cli_kmp_mapping {
    uint_8 code;
    uint_8 code_shift;
    uint_8 code_ctrl;
    uint_8 code_ctrl_shift;
    uint_8 code_alt;
    uint_8 code_alt_shift;
    uint_8 code_alt_ctrl;
    uint_8 code_alt_ctrl_shift;
    enum user_cli_kmp_modifier modifier;
    enum user_cli_kmp_special special;
};

const struct user_cli_kmp_mapping user_cli_kmp_default[SCANCODES_MAX];

uint_8 user_cli_kmp_defines(const struct user_cli_kmp_mapping *map, const uint_8 c);
