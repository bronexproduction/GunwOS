//
//  keybuf.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include <types.h>

struct user_cli_kbf_listener {
    void (*up)(const uint_8 c);
    void (*down)(const uint_8 c);
};

void user_cli_kbf_register(const struct user_cli_kbf_listener);
uint_8 user_cli_kbf_currModMask();
