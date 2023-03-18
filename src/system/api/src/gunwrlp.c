//
//  gunwrlp.c
//  GunwOS
//
//  Created by Artur Danielewski on 18.03.2023.
//

#include "_gunwrlp.h"
#include <gunwrlp.h>
#include <defs.h>

static struct gnwRunLoop rlp_main;
static bool rlp_started = false;

void runLoopStart() {
    rlp_started = true;
    while(1) {
        // go through the items and execute
    }
}

ptr_t runLoopGetMain() {
    return rlp_started ? (ptr_t)&rlp_main : nullptr;
}
