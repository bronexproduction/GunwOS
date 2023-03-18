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

void runLoopStart() {
    while(1) {
        // go through the items and execute
    }
}

ptr_t runLoopGetMain() {
    return (ptr_t)&rlp_main;
}
