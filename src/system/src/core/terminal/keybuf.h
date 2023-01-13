//
//  keybuf.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include "../../../../lib/stdgunw/types.h"

struct k_kbf_listener {
    void (*up)(const uint_8 c);
    void (*down)(const uint_8 c);
};

void k_kbf_register(const struct k_kbf_listener);
uint_8 k_kbf_currModMask();
