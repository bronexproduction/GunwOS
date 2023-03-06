//
//  keybuf.h
//  GunwOS
//
//  Created by Artur Danielewski on 13.03.2020.
//

#include <stdgunw/types.h>

struct c_kbf_listener {
    void (*up)(const uint_8 c);
    void (*down)(const uint_8 c);
};

void c_kbf_register(const struct c_kbf_listener);
uint_8 c_kbf_currModMask();
